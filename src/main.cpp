#include <M5Cardputer.h>

#include "Adafruit_NeoPixel.h"
#include "AppManager.h"
#include "Globals.h"
#include "Preferences.h"
#include "ScreenManager.h"
#include "Utils.h"
#include "apps/Calculadora/Calculadora.h"
#include "apps/Settings/Settings.h"

SemaphoreHandle_t canvasSemaphore;

void setup() {
    // Inicializa o M5Cardputer e outros componentes
    canvasSemaphore = xSemaphoreCreateMutex();
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 21, NEO_GRB + NEO_KHZ800);
    pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
    pixels.show();

    Preferences preferences;
    auto cfg = m5::M5Unified::config();
    M5Cardputer.begin(cfg, true);

    M5Cardputer.update();
    M5Cardputer.Display.setColorDepth(8);
    M5Cardputer.Display.setRotation(1);

    canvas.createSprite(240, 135);
    Utils::initCanvas();
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
