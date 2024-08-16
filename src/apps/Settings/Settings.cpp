#include "Settings.h"

#include "ScreenManager.h"

Settings::Settings() {
    // Inicialização do aplicativo
}

Settings::~Settings() {
    // Limpeza dos recursos
}

void Settings::onAppTick() {
    if (millis() - lastMillis > delay_t) {
        //printf("Testando o programa\n");
        lastMillis = millis();
    }

    // vTaskDelay(pdMS_TO_TICKS(1000));
    //  Lógica de atualização do aplicativo
}

void Settings::onAppOpen() {
    delay_t = 1000;
    lastMillis = millis();
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
