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
        state = newState;
        if (newState == State::OPEN) {
            digitalWrite(output, HIGH);
        } else if (newState == State::CLOSED) {
            digitalWrite(output, LOW);
        }
        Serial << beginl << blue << "Channel " << input << " set to state " << state << DI::endl;
    }
}
