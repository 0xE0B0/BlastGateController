#pragma once
#include <Arduino.h>
#include "DebugInterface.h"

class Channel {
public:
    Channel() = default;

    enum class State {
        INIT,
        OPEN,
        CLOSED
    };

    friend inline Print& operator<<(Print &stream, State s) {
        switch (s) {
            case State::INIT:   stream << "INIT"; break;
            case State::OPEN:   stream << "OPEN"; break;
            case State::CLOSED: stream << "CLOSED"; break;
        }
        return stream;
    }

    void init(uint8_t inputPin, uint8_t outputPin) {
        input = inputPin;
        output = outputPin;
        pinMode(input, INPUT_PULLUP);
        pinMode(output, OUTPUT);
        setOutputState(State::INIT);
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
    uint8_t output = 0;
    bool inputState = false;
    uint64_t inputDebounce = 0;
    State state = State::INIT;
    State newState = State::INIT;

};
