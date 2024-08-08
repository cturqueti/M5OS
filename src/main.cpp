#include <M5Cardputer.h>

#include "AppManager.h"
#include "apps/Calculadora/Calculadora.h"
#include "apps/Settings/Settings.h"

void setup() {
    // Inicializa o M5Cardputer e outros componentes
    M5.begin();
    M5.Lcd.setRotation(3);  // Ajuste a rotação do display conforme necessário

    // Inicializa o AppManager
    AppManager& appManager = AppManager::getInstance();

    // Cria instâncias dos aplicativos
    Calculadora* calc = new Calculadora();
    Settings* settings = new Settings();

    // Adiciona aplicativos ao AppManager
    appManager.addApp("Calculadora", calc);
    appManager.addApp("Settings", settings);

    // Abre o aplicativo de Calculadora
    appManager.openApp("Calculadora");
}

void loop() {
    AppManager& appManager = AppManager::getInstance();

    // Atualiza e desenha o aplicativo atual
    appManager.tickCurrentApp();
    appManager.draw();

    delay(10);  // Pequeno delay para reduzir o uso da CPU
}
