#pragma once
#include <Arduino.h>
#include "DebugInterface.h"
#include "Debounce.h"
#include "Channel.h"
#include "config.h"
#include "version.h"

template<size_t CHANNEL_COUNT>
class BlastGateController {
public:

    BlastGateController();

    void init(const uint8_t *chInputPins, const uint8_t *chOutputAPins, const uint8_t *chOutputBPins);
    void update();

    void debounce() {
        uint8_t input = 0;
        for (size_t i = 0; i < CHANNEL_COUNT; ++i)
            input |= ((digitalRead(inputPins[i]) == LOW) ? (1u << i) : 0);  // low active inputs
        deb.tick(input);
    }

private:
    const uint8_t *inputPins = nullptr;
    Channel channels[CHANNEL_COUNT];
    // debounce engine (10ms tick)
    Debounce<uint8_t, 10> deb{0, 0};

};

// Prevent implicit instantiation of the project specialization in other TUs
extern template class BlastGateController<PROJECT_CHANNEL_COUNT>;

