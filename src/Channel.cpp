#include "Channel.h"

static inline Print& beginl(Print &stream) {
    static constexpr const char name[] = "CH";
    return beginl<name>(stream);
}

void Channel::loop() {
    if (millis() % 10 == 0) {
        // run every 10 ms -> 640 ms debounce time
        auto in = digitalRead(input) == LOW;  // active low
        inputDebounce = (inputDebounce << 1) | (in ? 1 : 0);
        if (inputDebounce == UINT64_MAX) {
            inputState = true;
        } else if (inputDebounce == 0) {
            inputState = false;
        }
    }
    if (newState != state) {
        if (!transition) {
            if (newState == State::OFF) {
                digitalWrite(outputA, LOW);
                digitalWrite(outputB, LOW);
                state = newState;
            } else if (newState == State::OPEN) {
                // OPEN: set A high first, then after delay set B low
                digitalWrite(outputA, HIGH);
                transitionStart = millis();
                transition = true;
            } else if (newState == State::CLOSED) {
                // CLOSED: set A low first, then after delay set B high
                digitalWrite(outputA, LOW);
                transitionStart = millis();
                transition = true;
            }
        } else if (transition) {
            // wait for delay and complete transition
            if ((unsigned long)(millis() - transitionStart) >= SWITCH_DELAY_MS) {
                if (newState == State::OPEN) {
                    digitalWrite(outputB, LOW);
                } else if (newState == State::CLOSED) {
                    digitalWrite(outputB, HIGH);
                }
                state = newState;
                transition = false;
                transitionStart = 0;
                Serial << beginl << blue << "Channel " << input << " set to state " << state << DI::endl;
            }
        }
    }
}
