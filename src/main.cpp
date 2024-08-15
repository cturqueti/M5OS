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
// #include "services/Keyboard/Keyboard.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

void setup() {
    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set("AppManager", ESP_LOG_INFO);
    ServicesManager& serviceManager = ServicesManager::getInstance();

    AppManager& appManager = AppManager::getInstance();

    // Adiciona aplicativos ao AppManager
    appManager.addApp("Calculadora", new Calculadora());
    appManager.addApp("Settings", new Settings());
    appManager.addApp("Launcher", new Launcher());

    delay(2000);
    // servicesManager.openService("Keyboard");
    appManager.openApp("Launcher");
    appManager.openApp("Settings");
}

void loop() {
    // AppManager::getInstance().printDebugInfo();
    delay(2000);  // Pequeno delay para reduzir o uso da CPU
}
