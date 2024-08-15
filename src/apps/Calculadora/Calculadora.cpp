#include "Calculadora.h"

#include "ScreenManager.h"

static const char* TAG = "Calculadora";

Calculadora::Calculadora() : priority(2) {
}

Calculadora::~Calculadora() {
}

void Calculadora::onAppTick() {
    nloop -= 1;
    // ESP_LOGI(TAG, "Fazendo algo na calculadora");
    if (nloop < 1) {
        ESP_LOGI(TAG, "Fechando Launcher");
        AppManager::getInstance().closeApp("Calculadora");
    }
}

void Calculadora::onAppOpen() {
    ESP_LOGI(TAG, "Iniciando Calculadora");
    nloop = 800;

    const uint8_t* icon = getIcon();
    if (icon) {
        printf("Ícone carregado com sucesso.\n");
    } else {
        printf("Erro ao carregar ícone.\n");
    }
}

void Calculadora::onAppClose() {
    AppManager::getInstance().openApp("Launcher");
}

void Calculadora::draw() {
}

const uint8_t* Calculadora::getIcon() {
    return calc_icon;
}

size_t Calculadora::getIconSize() {
    return sizeof(calc_icon);
}