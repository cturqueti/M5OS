#include "WiFiManager.h"

#include "esp_log.h"
static const char* TAG = "SD Card";

// Construtor
WiFiManager::WiFiManager() : priority(99), scanRate(100) {}

// Destrutor
WiFiManager::~WiFiManager() {
}

void WiFiManager::onServiceOpen() {
}

void WiFiManager::onServiceTick() {
    if (millis() - lastMillis > scanRate) {
    }
}

void WiFiManager::onServiceClose() {
    if (isConnected()) {
        ESP_LOGI(TAG, "SD card has been successfully unmounted.");
    }
}

void WiFiManager::draw() {}

const uint8_t* WiFiManager::getIcon() {
    if (connected) {
        return wifiConnectedIcon;
    } else {
        return wifiDisconnectedIcon;
    }
}

size_t WiFiManager::getIconSize() {
    if (connected) {
        return sizeof(wifiConnectedIcon);
    } else {
        return sizeof(wifiDisconnectedIcon);
    }
}
