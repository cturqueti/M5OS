#include "SDManager.h"

#include "esp_log.h"
static const char* TAG = "SD Card";

// Construtor
SDManager::SDManager() : priority(99), scanRate(2000) {}

// Destrutor
SDManager::~SDManager() {
}

void SDManager::onServiceOpen() {
    // Inicializa o SPI com os pinos especificados
    SPI2.begin(sdCard.sck,   // CLK
               sdCard.miso,  // MISO
               sdCard.mosi,  // MOSI
               sdCard.cs);   // SS

    uint16_t timeout = 600;
    while (!SD.begin(sdCard.cs, SPI2) && timeout-- > 1) {
        delay(10);
    }
    if (timeout > 0) {
        ESP_LOGI(TAG, "MicroSd Ok");
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE) {
            ESP_LOGI(TAG, "No SD card attached");

        } else {
            if (cardType == CARD_MMC) {
                ESP_LOGI(TAG, "SD Card Type: MMC");
            } else if (cardType == CARD_SD) {
                ESP_LOGI(TAG, "SD Card Type: SDSC");
            } else if (cardType == CARD_SDHC) {
                ESP_LOGI(TAG, "SD Card Type: SDHC");
            } else {
                ESP_LOGI(TAG, "SD Card Type: UNKNOWN");
            }
            cardSize = SD.cardSize() / 1024 * 1024;
            ESP_LOGI(TAG, "SD Card Size: %lluMB", cardSize);
        }
    } else {
        ESP_LOGI(TAG, "Card Mount Failed");
    }
    lastMillis = millis();
}

void SDManager::onServiceTick() {
    if (millis() - lastMillis > scanRate) {
        if (!isInitialized()) {
            ESP_LOGW(TAG, "SD card detected, reinitializing...");
            onServiceOpen();  // Tenta reinicializar o cartão SD
        } else if (totalSpace() == 0) {
            // O cartão SD não está presente ou foi removido
            if (isInitialized()) {
                SD.end();
                ESP_LOGE(TAG, "SD card removed or not accessible.");
            }
        } else {
            ESP_LOGI(TAG, "SD card working...");
            lastMillis = millis();
        }
    }
}

void SDManager::onServiceClose() {
    if (isInitialized()) {
        SD.end();
        ESP_LOGI(TAG, "SD card has been successfully unmounted.");
    }
}

void SDManager::draw() {}

const uint8_t* SDManager::getIcon() {
    return 0;
}

size_t SDManager::getIconSize() {
    return 0;
}