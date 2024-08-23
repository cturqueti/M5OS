#include "Calculadora.h"

static const char* TAG = "Calculadora";

Calculadora::Calculadora() : priority(2) {
}

Calculadora::~Calculadora() {
}

void Calculadora::onAppOpen() {
    nloop = 800;

    ESP_LOGI(TAG, "Iniciando delay de 800");

    const uint8_t* icon = getIcon();
    if (icon) {
        printf("Ícone carregado com sucesso.\n");
    } else {
        printf("Erro ao carregar ícone.\n");
    }
}

void Calculadora::onAppTick() {
    nloop -= 1;
    if (nloop % 10 == 0) {
        ESP_LOGI(TAG, "Rodando");
    }
    if (nloop < 1) {
        AppManager::getInstance().closeApp("Calculadora");
    }
}

void Calculadora::onAppClose() {
    ESP_LOGI(TAG, "Fechando Calculadora");
    nloop = 800;
    ESP_LOGI(TAG, "Abrindo Launcher");
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