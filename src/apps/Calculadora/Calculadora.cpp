#include "Calculadora.h"

#include "ScreenManager.h"

Calculadora::Calculadora() : tempCanvas(&M5.Lcd) {
    tempCanvas.createSprite(240, 135);
}

Calculadora::~Calculadora() {
    tempCanvas.deleteSprite();
}

void Calculadora::onAppTick() {
    printf("Testando o programa");
    delay(1000);
    // Lógica de atualização do aplicativo
}

void Calculadora::onAppOpen() {
    StatusBar::draw();
    DrawingArea area = StatusBar::area;
    tempCanvas.createSprite(area.width, area.height);
}

void Calculadora::onAppClose() {
    // apps.clear();
}

void Calculadora::draw() {
    tempCanvas.fillSprite(TFT_BLACK);  // Limpa o canvas temporário
    DrawingArea area = StatusBar::area;

    tempCanvas.setTextSize(1);
    tempCanvas.setTextColor(TFT_GREEN);
    tempCanvas.setCursor(5, 5);
    tempCanvas.print("Calculadora");

    // Sincroniza a operação com o canvas definitivo usando o semáforo global
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        // Copia o conteúdo do canvas temporário para o canvas definitivo
        tempCanvas.pushSprite(&canvas, area.xStart, area.yStart);

        // Libera o semáforo após o uso
        xSemaphoreGive(canvasSemaphore);
        canvas.pushSprite(0, 0);
    }
}
