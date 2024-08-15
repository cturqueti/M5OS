#include "Launcher.h"

#include "esp_log.h"

static const char* TAG = "Launcher";

Launcher::Launcher() {
}

Launcher::~Launcher() {
}

void Launcher::onAppOpen() {
    ESP_LOGI(TAG, "Iniciando Launcher");
    nloop = 800;
}

void Launcher::onAppClose() {
    ESP_LOGI(TAG, "Abrindo Calculadora");
    AppManager::getInstance().openApp("Calculadora");
}

void Launcher::onAppTick() {
    nloop -= 1;
    // ESP_LOGI(TAG, "Fazendo algo no Launcher");
    if (nloop < 1) {
        ESP_LOGI(TAG, "Fechando Launcher");
        AppManager::getInstance().closeApp("Launcher");
    }
}

void Launcher::draw() {
}
