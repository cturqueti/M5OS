// #include <M5Cardputer.h>
#include <esp_heap_caps.h>

#include "../assets/m5os.h"
#include "Adafruit_NeoPixel.h"
#include "AppManager.h"
#include "GlobalDisplay.h"
#include "Globals.h"
#include "Preferences.h"
#include "SD.h"
#include "SPI.h"
// #include "ScreenManager.h"
#include "ServicesManager.h"
#include "Utils.h"
#include "apps/Calculadora/Calculadora.h"
#include "apps/Launcher/Launcher.h"
#include "apps/Settings/Settings.h"
#include "services/MqttManager/MqttManager.h"
#include "services/SDManager/SDManager.h"
#include "services/ScreenManager/ScreenManager.h"
#include "services/ScreenManager/TitleBar.h"
#include "services/WifiManager/WiFiManager.h"
// #include "services/Keyboard/Keyboard.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

void setup() {
    // Serial.begin(115200);
    delay(2000);
    printf("Iniciando M5OS\n");
    Preferences preferences;
    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set("AppManager", ESP_LOG_INFO);

    GlobalDisplay& globalsDisplay = GlobalDisplay::getInstance();

    // auto cfg = m5::M5Unified::config();
    // M5Cardputer.begin(cfg, true);
    //  SPI2.begin(
    // m5::M5Unified::getPin(m5::pin_name_t::sd_spi_sclk),
    //      m5::M5Unified::getPin(m5::pin_name_t::sd_spi_miso),
    //      m5::M5Unified::getPin(m5::pin_name_t::sd_spi_mosi),
    //      m5::M5Unified::getPin(m5::pin_name_t::sd_spi_ss));
    //  delay(1000);
    //  if (!SD.begin(m5::M5Unified::getPin(m5::pin_name_t::sd_spi_ss), SPI2)) {
    //      printf("SD Card Mount Failed\n");
    //      printf("You don't need an SD anyway\n");
    //  } else {
    //      printf("MicroSd Ok\n");
    //  }
    // SDManager::getInstance().onServiceOpen();
    ServicesManager& serviceManager = ServicesManager::getInstance();
    serviceManager.addService("SDManager", new SDManager());
    serviceManager.addService("WiFiManager", new WiFiManager());
    serviceManager.addService("MqttManager", new MqttManager());
    serviceManager.addService("TitleBar", new TitleBar());
    //  serviceManager.addService("ScreenManager", new ScreenManager());

    // serviceManager.openService("SDManager");
    // serviceManager.openService("WiFiManager");
    // serviceManager.openService("MqttManager"); //falta terminar
    serviceManager.openService("TitleBar");

    AppManager& appManager = AppManager::getInstance();

    // Adiciona aplicativos ao AppManager
    appManager.addApp("Calculadora", new Calculadora());
    appManager.addApp("Settings", new Settings());
    appManager.addApp("Launcher", new Launcher());

    // servicesManager.openService("Keyboard");
    appManager.openApp("Launcher");
    // appManager.openApp("Settings");
}

void loop() {
    // AppManager::getInstance().printDebugInfo();
    // GlobalDisplay::getInstance().canvas.pushSprite(0, 0);
    delay(2000);  // Pequeno delay para reduzir o uso da CPU
}
