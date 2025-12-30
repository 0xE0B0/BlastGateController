#include "BlastGateController.h"

static inline Print& beginl(Print &stream) {
    static constexpr const char name[] = "BGC";
    return beginl<name>(stream);
}

BlastGateController::BlastGateController() {}

void BlastGateController::begin() {
    Serial << magenta << F("BlastGateController v");
    Serial << magenta << BGC_VERSION_MAJOR << F(".") << BGC_VERSION_MINOR << F(".") << BGC_VERSION_SUB;
    Serial << magenta << F(" (") <<  __TIMESTAMP__  << F(")") << DI::endl;

    delay(200);  // button debounce must have detect valid state before proceeding

}
void BlastGateController::update() {
    debounce();
}