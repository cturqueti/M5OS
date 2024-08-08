#include "Settings.h"

#include <M5Cardputer.h>

#include "Globals.h"
#include "ScreenManager.h"

Settings::Settings() {
    // Inicialização do aplicativo
}

Settings::~Settings() {
    // Limpeza dos recursos
}

void Settings::tick() {
    printf("Testando o programa");
    delay(1000);
    // Lógica de atualização do aplicativo
}

void Settings::draw() {
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        canvas.fillSprite(TFT_BLACK);  // Limpa o canvas
        canvas.setTextSize(2);
        canvas.setTextColor(TFT_WHITE);
        canvas.setCursor(10, 10);
        canvas.print("Settings");

        // Adicione mais desenho aqui

        canvas.pushSprite(0, 0);  // Desenha o canvas no display
        xSemaphoreGive(canvasSemaphore);
    }
}
