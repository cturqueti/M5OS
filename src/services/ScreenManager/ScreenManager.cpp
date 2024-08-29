#include "ScreenManager.h"

#include "esp_log.h"

const char* ScreenManager::TAG = "ScreenManager";

ScreenManager::ScreenManager()
    : canvas(*GlobalDisplay::getInstance().getCanvas()) {
}

ScreenManager::~ScreenManager() {
    // canvas.deleteSprite();
}

void ScreenManager::onServiceOpen() {
    // if (xSemaphoreTake(onServiceOpenSemaphore, portMAX_DELAY) == pdTRUE) {
    //     xSemaphoreGive(onServiceOpenSemaphore);
    // }
}

void ScreenManager::onServiceTick() {
    // canvas.pushSprite(0, 0);
}

void ScreenManager::onServiceClose() {
    ESP_LOGI(TAG, "Fechando a tela.");
}

void ScreenManager::draw() {
    canvas.pushSprite(0, 0);
}

const uint8_t* ScreenManager::getIcon() {
    return 0;
}

size_t ScreenManager::getIconSize() {
    return 0;
}
