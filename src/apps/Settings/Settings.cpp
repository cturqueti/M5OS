#include "Settings.h"

#include <M5Cardputer.h>

Settings::Settings() {
    // Inicialização do aplicativo
}

Settings::~Settings() {
    // Limpeza dos recursos
}

void Settings::tick() {
    // Lógica de atualização do aplicativo
}

void Settings::draw() {
    M5.Lcd.clear();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.print("Settings");
    // Desenhe a interface de configurações aqui
}
