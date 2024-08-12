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

SemaphoreHandle_t canvasSemaphore;

void base(const String& text = "") {
    canvas.fillScreen(BLACK);
    canvas.setTextSize(1);
    canvas.drawRoundRect(5, 5, 230, 125, 7, WHITE);
    canvas.drawFastHLine(5, 20, 230, WHITE);
    canvas.drawString("M5OS", 10, 9);
    canvas.drawRightString(text, 230, 9);
}

void m5OSLogoAnimation(int percent) {
    // base();
    //  Draw the rabbitOS logo partially as by the given percent
    int width = 75;
    // get top left coordinates for img (screen is M5Cardputer.Display.width() x M5Cardputer.Display.height() pixels)
    int x = ((240 - width) / 2);
    int y = ((135 - width) / 2) + 5;
    canvas.drawPng(m5os, sizeof(m5os), x, y, width, width);
    // draw gradient line as bottom border based on percent
    int lineWidth = 230 * percent / 100;
    for (int i = 0; i < 5; i++) {
        canvas.drawGradientHLine(5, 130 - 1 - i, lineWidth + 3, GREEN, CYAN);
    }
    canvas.drawCenterString("Press any key to skip", 120, 110);
    canvas.pushSprite(0, 0);
}

void setup() {
    // Inicializa o M5Cardputer e outros componentes
    canvasSemaphore = xSemaphoreCreateMutex();
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 21, NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
    pixels.show();

    Preferences preferences;
    auto cfg = m5::M5Unified::config();
    M5Cardputer.begin(cfg, true);

    M5Cardputer.update();
    M5Cardputer.Display.setColorDepth(8);
    M5Cardputer.Display.setRotation(1);

    canvas.createSprite(240, 135);

    // base();
    // for (int i = 0; i < 100; i++) {
    //     m5OSLogoAnimation(i);
    //     delay(30);
    //     M5Cardputer.update();
    //     if (M5Cardputer.Keyboard.isPressed()) {
    //         break;
    //     }
    // }

    Utils::initCanvas();

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

    servicesManager.openService("Keyboard");
    appManager.openApp("Launcher");
}

void loop() {
    AppManager& appManager = AppManager::getInstance();

    // Atualiza e desenha o aplicativo atual

    appManager.draw();
    canvas.pushSprite(0, 0);
    // printf("Memória livre: %d bytes\n", ESP.getFreeHeap());

    delay(200);  // Pequeno delay para reduzir o uso da CPU
}
