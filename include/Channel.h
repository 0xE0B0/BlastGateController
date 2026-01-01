#pragma once
#include <Arduino.h>

class Channel {
public:
    Channel() = default;

    enum class State {
        OFF,
        OPEN,
        CLOSED
    };

    // configure output pins (set pin numbers)
    void init(uint8_t outputAPin, uint8_t outputBPin) {
        outputA = outputAPin;
        outputB = outputBPin;
        pinMode(outputA, OUTPUT);
        pinMode(outputB, OUTPUT);
        setOutputState(State::OFF);
    }

    void setOutputState(State newState) {
        state = newState;
        switch (state) {
            case State::OFF:
                digitalWrite(outputA, LOW);
                digitalWrite(outputB, LOW);
                break;
            case State::OPEN:
                digitalWrite(outputA, HIGH);
                delay(SWITCH_DELAY_MS);
                digitalWrite(outputB, LOW);
                delay(SWITCH_DELAY_MS);
                break;
            case State::CLOSED:
                digitalWrite(outputA, LOW);
                delay(SWITCH_DELAY_MS);
                digitalWrite(outputB, HIGH);
                delay(SWITCH_DELAY_MS);
                break;
        }
    }

    State getOutputState() const {
        return state;
    }

private:
    uint8_t outputA = 0;
    uint8_t outputB = 0;
    State state = State::OFF;

    static constexpr uint8_t SWITCH_DELAY_MS = 5;
};
