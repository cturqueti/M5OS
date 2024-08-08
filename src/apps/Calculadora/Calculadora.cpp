#include "Calculadora.h"

#include <M5Cardputer.h>

#include "Globals.h"
#include "ScreenManager.h"

Calculadora::Calculadora() {
    // Inicialização do aplicativo
}

Calculadora::~Calculadora() {
    // Limpeza dos recursos
}

void Calculadora::tick() {
    printf("Testando o programa");
    delay(1000);
    // Lógica de atualização do aplicativo
}

void Calculadora::draw() {
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        StatusBar::draw();

        // Adicione mais desenho aqui
        M5Canvas& canvas = ScreenManager::getCanvas();

        // canvas.fillSprite(TFT_BLACK);  // Limpa o canvas
        canvas.setTextSize(2);
        canvas.setTextColor(TFT_WHITE);
        canvas.setCursor(50, 67);
        canvas.print("Calculadora");
        canvas.pushSprite(0, 0);  // Desenha o canvas no display
        xSemaphoreGive(canvasSemaphore);
    }
}
