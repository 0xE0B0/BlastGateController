#pragma once
#include <Arduino.h>
#include "DebugInterface.h"
#include "Channel.h"
#include "config.h"
#include "version.h"

template<size_t CHANNEL_COUNT>
class BlastGateController {
public:

    BlastGateController();

    void init(const uint8_t vacuumOutputPin, const uint8_t *chInputPins, const uint8_t *chOutputAPins, const uint8_t *chOutputBPins);
    void loop();

private:
    Channel channels[CHANNEL_COUNT];
    uint8_t vacuumPin = 0;

};
