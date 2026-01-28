#include <Arduino.h>
#include "BlastGateController.h"
#include "LEDControl.h"
#include "config.h"
#include "version.h"
#include <WifiManager.h>

#define STRINGIFY(x) #x
#define STRINGIFY_EXP(x) STRINGIFY(x)

// WiFi manager for OTA update and WiFi configuration
WiFiManager wm;

// Firmware version info for web portal
WiFiManagerParameter fwInfo("<p>Firmware Version: v"
    STRINGIFY_EXP(BGC_VERSION_MAJOR) "." STRINGIFY_EXP(BGC_VERSION_MINOR) "." STRINGIFY_EXP(BGC_VERSION_SUB)
    "<br>Build Time: " __TIMESTAMP__ "</p>");

// Access point name
static constexpr const char* APName = "BlastGateController_AP";

// Status LED
LEDControl led(LED_PIN, true);

BlastGateController<PROJECT_CHANNEL_COUNT> controller;

// Wi-Fi manager + OTA update mode
static void startWiFiManager() {
    WiFi.mode(WIFI_STA);
    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);
    wm.setDarkMode(true);

    // Add firmware details as custom param to web portal
    wm.addParameter(&fwInfo);

    bool res = wm.autoConnect(APName);
    if (!res) {
        Serial << yellow << "started config portal in AP mode, IP: " << WiFi.softAPIP() << DI::endl;
        led.setState(LEDControl::LED_FLASH_FAST);
    } else {
        Serial << green << "started config portal with WiFi connection, IP: " << WiFi.localIP() << DI::endl;
        wm.startWebPortal();
    }
}

void setup() {
    Serial.begin(debugBaudRate);
    Serial << magenta << F("BlastGateController v");
    Serial << magenta << BGC_VERSION_MAJOR << F(".") << BGC_VERSION_MINOR << F(".") << BGC_VERSION_SUB;
    Serial << magenta << F(" (") <<  __TIMESTAMP__  << F(")") << DI::endl;

    led.setState(LEDControl::LED_FLASH_SLOW);

    // all buttons pressed at startup for one second enters config mode
    // to configure Wifi credentials and run OTA firmware update
    bool anyButtonPressed = false;
    for (size_t i = 0; i < PROJECT_CHANNEL_COUNT; ++i) {
        pinMode(CH_INPUT_PINS[i], INPUT_PULLUP);
        delay(1);
        if (digitalRead(CH_INPUT_PINS[i]) == LOW)
            anyButtonPressed = true;
    }

    auto allButtonsPressed = []() {
        bool pressed = true;
        for (int i = 0; i < 1000; i++) {
            for (size_t i = 0; i < PROJECT_CHANNEL_COUNT; ++i) {
                if (digitalRead(CH_INPUT_PINS[i]) == HIGH) {
                    pressed = false;
                    break;
                }
            }
            delay(1);
        }
        return pressed;
    };

    if (anyButtonPressed && allButtonsPressed()) {
        Serial << yellow << "buttons held for one second, entering config mode" << DI::endl;
        startWiFiManager();
    } else {   
        led.setState(LEDControl::LED_ON);
        controller.init(VACUUM_OUTPUT_PIN, CH_INPUT_PINS, CH_OUTPUT_PINS);
    }
}

void loop() {
    controller.loop();
    wm.process();
    led.update();
}
