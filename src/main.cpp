#include <Arduino.h>
#include "BlastGateController.h"
#include "config.h"

BlastGateController<PROJECT_CHANNEL_COUNT> controller;

void setup() {
    Serial.begin(debugBaudRate);
    controller.init(VACUUM_OUTPUT_PIN, CH_INPUT_PINS, CH_OUTPUT_A_PINS, CH_OUTPUT_B_PINS);
}

void loop() {
    controller.loop();
}
