#include "Settings.h"

#include "ScreenManager.h"

Settings::Settings() {
    // Inicialização do aplicativo
}

Settings::~Settings() {
    // Limpeza dos recursos
}

void Settings::onAppTick() {
    printf("Testando o programa");
    delay(1000);
    // Lógica de atualização do aplicativo
}

void Settings::onAppOpen() {
}

void Settings::onAppClose() {
    // apps.clear();
}

void Settings::draw() {
}

const uint8_t* Settings::getIcon() {
    return settings_icon;
}

size_t Settings::getIconSize() {
    return sizeof(settings_icon);
}
