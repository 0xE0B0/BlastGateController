#include "BlastGateController.h"
#include "DebugInterface.h"
#include "config.h"

static inline Print& beginl(Print &stream) {
    static constexpr const char name[] = "BGC";
    return beginl<name>(stream);
}

template<size_t CHANNEL_COUNT>
BlastGateController<CHANNEL_COUNT>::BlastGateController() {}

template<size_t CHANNEL_COUNT>
void BlastGateController<CHANNEL_COUNT>::init(const uint8_t vacuumOutputPin, const uint8_t *chInputPins, const uint8_t *chOutputPins) {
    Serial << magenta << F("BlastGateController v");
    Serial << magenta << BGC_VERSION_MAJOR << F(".") << BGC_VERSION_MINOR << F(".") << BGC_VERSION_SUB;
    Serial << magenta << F(" (") <<  __TIMESTAMP__  << F(")") << DI::endl;

    for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
        channels[i].init(chInputPins[i], chOutputPins[i]);
    }
    vacuum.init(vacuumOutputPin);
    Serial << beginl << "Initialized with " << CHANNEL_COUNT << " channels" << DI::endl;
}

template<size_t CHANNEL_COUNT>
void BlastGateController<CHANNEL_COUNT>::loop() {
    // Step 1: let each channel sample inputs
    for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
        channels[i].loop();
    }

    // Step 2: compute which channels currently request to be open
    bool request[CHANNEL_COUNT]{};
    bool anyRequest = false;
    for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
        request[i] = channels[i].getInputState();
        if (request[i]) anyRequest = true;
    }

    // Step 3: open channels with active input
    for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
        if (request[i]) {
            if (channels[i].getOutputState() != Channel::State::OPEN) {
                Serial << beginl << "open channel " << i << DI::endl;
                channels[i].setOutputState(Channel::State::OPEN);
            }
        }
    }

    // Step 4: if there is at least one request, close channels whose inputs are inactive
    // This avoids unnecessary movements, closing only occurs when another channel is requested
    if (anyRequest) {
        uint8_t activeChannels = 0;
        for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
            if (request[i]) {
                activeChannels |= (1 << i);
            }
        }
        vacuum.switchTo(true, activeChannels);
        for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
            if (!request[i] && (channels[i].getOutputState() == Channel::State::OPEN)) {
                Serial << beginl << "close channel " << i << DI::endl;
                channels[i].setOutputState(Channel::State::CLOSED);
            }
        }
    } else {
        vacuum.switchTo(false, 0);
    }
}

template class BlastGateController<PROJECT_CHANNEL_COUNT>;
