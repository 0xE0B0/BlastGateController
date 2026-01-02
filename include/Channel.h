#pragma once
#include <Arduino.h>
#include "DebugInterface.h"

class Channel {
public:
    Channel() = default;

    enum class State {
        OFF,
        OPEN,
        CLOSED
    };

    friend inline Print& operator<<(Print &stream, State s) {
        switch (s) {
            case State::OFF:   stream << "OFF"; break;
            case State::OPEN:  stream << "OPEN"; break;
            case State::CLOSED:stream << "CLOSED"; break;
        }
        return stream;
    }

    void init(uint8_t inputPin, uint8_t outputAPin, uint8_t outputBPin) {
        input = inputPin;
        outputA = outputAPin;
        outputB = outputBPin;
        pinMode(input, INPUT_PULLUP);
        pinMode(outputA, OUTPUT);
        pinMode(outputB, OUTPUT);
        setOutputState(State::OFF);
    }

    void loop();

    inline void setOutputState(State state) {
        newState = state;
    }

    inline bool getInputState() const {
        return inputState;
    }

    inline State getOutputState() const {
        return state;
    }

private:
    uint8_t input = 0;
    uint8_t outputA = 0;
    uint8_t outputB = 0;
    bool inputState = false;
    uint64_t inputDebounce = 0;
    State state = State::OFF;
    State newState = State::OFF;

    unsigned long transitionStart = 0;
    bool transition = false; // false = idle, true = waiting for second output

    // delay between switching outputs to avoid shorts
    static constexpr uint8_t SWITCH_DELAY_MS = 5;
};
