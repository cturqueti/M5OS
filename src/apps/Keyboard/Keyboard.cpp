#include "Keyboard.h"

Keyboard::Keyboard() : inputText(""), tempCanvas(&M5.Lcd) {
    tempCanvas.createSprite(240, 135);
}

Keyboard::~Keyboard() {
    tempCanvas.deleteSprite();
}

void Keyboard::onAppOpen() {
    tempCanvas.createSprite(30, 13);
    // tempCanvas.fillSprite(TFT_GREEN);
}

void Keyboard::onAppClose() {
    // Limpa a tela ao fechar
    // M5Cardputer.Display.clear();
}

void Keyboard::onAppTick() {
    // Lida com a entrada do usuário
    M5Cardputer.update();
    handleInput();
    // draw();
}

void Keyboard::draw() {
    // tempCanvas.fillSprite(TFT_GREEN);
    tempCanvas.fillCircle(7, 7, 5, TFT_ORANGE);
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        // Copia o conteúdo do canvas temporário para o canvas definitivo
        tempCanvas.pushSprite(&canvas, 120, 7);

        // Libera o semáforo após o uso
        xSemaphoreGive(canvasSemaphore);
        canvas.pushSprite(0, 0);
    }
}

void Keyboard::handleInput() {
}

void Keyboard::drawText() {
    // M5Cardputer.Display.clear();
    // M5Cardputer.Display.setCursor(0, 0);
    // M5Cardputer.Display.print("Digite: ");
    // M5Cardputer.Display.println(inputText);
}
