#pragma once
#include <Arduino.h>
#include "DebugInterface.h"
#include "Debounce.h"
#include "version.h"

class BlastGateController {
public:
    BlastGateController();

    void begin();
    void update();

    void debounce() {
        auto input = (((digitalRead(inputPin) == LOW) ? CMD_INPUT_1 : 0) |
                      ((digitalRead(inputPin) == LOW) ? CMD_INPUT_2 : 0) |
                      ((digitalRead(inputPin) == LOW) ? CMD_INPUT_3 : 0) |
                      ((digitalRead(inputPin) == LOW) ? CMD_INPUT_4 : 0));
        deb.tick(input);
    }

private:

    // pin identifiers
    static constexpr uint8_t CMD_INPUT_1 = 1;
    static constexpr uint8_t CMD_INPUT_2 = 2;
    static constexpr uint8_t CMD_INPUT_3 = 4;
    static constexpr uint8_t CMD_INPUT_4 = 8;

    // hardware
    uint8_t inputPin;
    Debounce<uint8_t, 10> deb = Debounce<uint8_t>(0, 0);

    inline bool getInputState() { return deb.getKeyState(CMD_INPUT_1); }
 
};
