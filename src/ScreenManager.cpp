#include "ScreenManager.h"

#include "AppManager.h"

M5Canvas canvas = M5Canvas(&M5Cardputer.Lcd);
DrawingArea StatusBar::area;

StatusBar::StatusBar() {
}

StatusBar::~StatusBar() {
}

void StatusBar::draw(bool force) {
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        canvas.fillSprite(BLACK);  // Limpa o sprite
        canvas.setTextSize(1);
        canvas.setTextColor(WHITE);
        // Desenha o retângulo arredondado
        canvas.drawRoundRect(5, 5, 230, 125, 7, WHITE);

        // Define a margem interna
        int margin = 1;

        // Calcula a área útil para desenho
        area.xStart = 5 + margin;        // Posição x inicial para desenho
        area.yStart = 5 + margin;        // Posição y inicial para desenho
        area.width = 230 - 2 * margin;   // Largura útil
        area.height = 125 - 2 * margin;  // Altura útil

        // Desenha uma linha horizontal dentro da área útil
        canvas.drawFastHLine(area.xStart, area.yStart + 15, area.width, WHITE);

        // Desenha uma string dentro da área útil
        canvas.drawString(AppManager::getInstance().getCurrentAppName(), area.xStart + 5, area.yStart + 5);

        canvas.drawRightString("M5OS", area.xStart + area.width - 5, area.yStart + 5);

        xSemaphoreGive(canvasSemaphore);
        area.yStart += 15 + margin;  // Atualiza a posição inicial y para a próxima linha, se necessário
        area.height -= area.yStart;
    }
}

M5Canvas& ScreenManager::getCanvas() {
    return canvas;
}
