// #include <M5Cardputer.h>
#include <esp_heap_caps.h>

#include "../assets/m5os.h"
#include "Adafruit_NeoPixel.h"
#include "AppManager.h"
#include "GlobalDisplay.h"
#include "Preferences.h"
#include "SD.h"
#include "SPI.h"
#include "core/Globals.h"
// #include "ScreenManager.h"
#include "ServicesManager.h"
#include "Utils.h"
#include "apps/Calculadora/Calculadora.h"
#include "apps/Launcher/Launcher.h"
#include "apps/Settings/Settings.h"
#include "services/Keyboard/Keyboard.h"
#include "services/MqttManager/MqttManager.h"
#include "services/SDManager/SDManager.h"
#include "services/ScreenManager/ScreenManager.h"
#include "services/ScreenManager/TitleBar.h"
#include "services/WifiManager/WiFiManager.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

void setup() {
    // Serial.begin(115200);
    delay(2000);
    printf("Iniciando M5OS\n");
    Preferences preferences;
    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set("AppManager", ESP_LOG_INFO);

    GlobalDisplay& globalsDisplay = GlobalDisplay::getInstance();

    ServicesManager& serviceManager = ServicesManager::getInstance();
    serviceManager.addService("SDManager", new SDManager());
    serviceManager.addService("WiFiManager", new WiFiManager());
    serviceManager.addService("MqttManager", new MqttManager());
    serviceManager.addService("TitleBar", new TitleBar());
    serviceManager.addService("Keyboard", new Keyboard());
    serviceManager.addService("ScreenManager", new ScreenManager());

    // serviceManager.openService("SDManager");
    // serviceManager.openService("WiFiManager");
    // serviceManager.openService("MqttManager"); //falta terminar
    serviceManager.openService("ScreenManager");
    serviceManager.openService("TitleBar");
    serviceManager.openService("Keyboard");

    AppManager& appManager = AppManager::getInstance();

    // Adiciona aplicativos ao AppManager
    appManager.addApp("Calculadora", new Calculadora());
    appManager.addApp("Settings", new Settings());
    appManager.addApp("Launcher", new Launcher());

    // servicesManager.openService("Keyboard");
    // appManager.openApp("Launcher");
    // appManager.openApp("Calculadora");
    appManager.openApp("Settings");
}

void loop() {
    // AppManager::getInstance().printDebugInfo();
    // GlobalDisplay::getInstance().canvas.pushSprite(0, 0);
    delay(2000);  // Pequeno delay para reduzir o uso da CPU
}
