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
void BlastGateController<CHANNEL_COUNT>::init(const uint8_t *chInputPins, const uint8_t *chOutputAPins, const uint8_t *chOutputBPins) {
    inputPins = chInputPins;
    Serial << magenta << F("BlastGateController v");
    Serial << magenta << BGC_VERSION_MAJOR << F(".") << BGC_VERSION_MINOR << F(".") << BGC_VERSION_SUB;
    Serial << magenta << F(" (") <<  __TIMESTAMP__  << F(")") << DI::endl;

    delay(200);  // button debounce must have detect valid state before proceeding
    for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
        pinMode(inputPins[i], INPUT_PULLUP);
        channels[i].init(chOutputAPins[i], chOutputBPins[i]);
    }
}

template<size_t CHANNEL_COUNT>
void BlastGateController<CHANNEL_COUNT>::update() {

}

// Explicit instantiation for the project channel count (defined in ProjectConfig.h)
template class BlastGateController<PROJECT_CHANNEL_COUNT>;

