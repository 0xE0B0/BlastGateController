#pragma once
#include <Arduino.h>
#include "DebugInterface.h"
#include <ESP8266WiFi.h>
#include <espnow.h>

class VacuumCleaner {

public:
    VacuumCleaner() : pin(0xff), lastState(false), initialized(false) {}

    void init(uint8_t vacuumPin);
    void switchTo(bool active, uint8_t activeChannels);
    void setupEspNowHost();
    void sendState(bool state, uint8_t activeChannels);
    static void packetSent_cb(uint8_t* mac, uint8_t sendStatus);

private:
    uint8_t pin;  // default: 0xFF = no pin assigned
    bool lastState;
    bool initialized = false;

    class __attribute__((packed)) Datagram {
    public:
        uint32_t magic;
        uint8_t switchState;
        uint8_t activeChannels;
        Datagram(bool state, uint8_t channels) : magic(MAGIC_KEY), switchState(state ? 1 : 0), activeChannels(channels) {}
    private:
        // Magic key to identify datagrams
        static constexpr uint32_t MAGIC_KEY = 0xDEADBEEF;
    };
};
