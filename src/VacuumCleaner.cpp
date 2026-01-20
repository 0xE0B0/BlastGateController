#include "VacuumCleaner.h"

static inline Print& beginl(Print &stream) {
    static constexpr const char name[] = "VAC";
    return beginl<name>(stream);
}

void VacuumCleaner::init(uint8_t vacuumPin) {
    pin = vacuumPin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    lastState = false;
}

void VacuumCleaner::packetSent_cb(uint8_t* mac, uint8_t sendStatus) {
    if (sendStatus != 0) {
        Serial << beginl << red << "esp-now packet sending failed (status=" << int(sendStatus) << ")" << DI::endl;
    }
}

void VacuumCleaner::setupEspNowHost() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    initialized = (esp_now_init() == 0);
    if (!initialized) {
        Serial << beginl << red << "esp-now init failed" << DI::endl;
    } else {
        Serial << beginl << green << "esp-now host ready" << DI::endl;
        esp_now_register_send_cb([](uint8_t* mac, uint8_t status) { VacuumCleaner::packetSent_cb(mac, status); });
        esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    }
}

void VacuumCleaner::switchTo(bool active, uint8_t activeChannels) {
    if (pin != 0xff) {
        if (active != lastState) {
            digitalWrite(pin, active ? HIGH : LOW);
            Serial << beginl << blue << "switch " << (active ? "on" : "off") << DI::endl;
            sendState(active, activeChannels);
            lastState = active;
        }
    }
}

void VacuumCleaner::sendState(bool state, uint8_t activeChannels) {
    if (!initialized)
        return;  // esp-now not initialized

    Datagram data(state, activeChannels);
    uint8_t broadcastAddr[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    auto result = esp_now_send(broadcastAddr, (uint8_t*)&data, sizeof(data));
    if (result == 0) {
        Serial << beginl << yellow << "esp-now broadcast packet to switch " << (state ? "on" : "off") << DI::endl;
    } else {
        Serial << beginl << red << "esp-now send failed" << DI::endl;
    }
}
