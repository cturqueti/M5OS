#include <M5Cardputer.h>
#include <esp_heap_caps.h>

#include "../assets/m5os.h"
#include "Adafruit_NeoPixel.h"
#include "AppManager.h"
#include "Globals.h"
#include "Preferences.h"
#include "ScreenManager.h"
#include "ServicesManager.h"
#include "Utils.h"
#include "apps/Calculadora/Calculadora.h"
#include "apps/Launcher/Launcher.h"
#include "apps/Settings/Settings.h"
#include "services/Keyboard/Keyboard.h"

void setup() {
    // Inicializa o M5Cardputer e outros componentes

    Preferences preferences;

    // Inicia o ServiceManager
    ServicesManager& servicesManager = ServicesManager::getInstance();

    // Adiciona serviços ao ServiceManager
    servicesManager.addService("Keyboard", new Keyboard());

    // Inicializa o AppManager
    AppManager& appManager = AppManager::getInstance();

    // Adiciona aplicativos ao AppManager
    appManager.addApp("Calculadora", new Calculadora());
    appManager.addApp("Settings", new Settings());
    appManager.addApp("Launcher", new Launcher());

    // servicesManager.openService("Keyboard");
    appManager.openApp("Launcher");
}

void loop() {
    delay(1000);  // Pequeno delay para reduzir o uso da CPU
}
