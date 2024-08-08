#include "ScreenManager.h"

#include "AppManager.h"

M5Canvas canvas = M5Canvas(&M5Cardputer.Lcd);

void StatusBar::draw(bool force) {
    canvas.fillSprite(BLACK);  // Limpa o sprite
    canvas.setTextSize(1);
    canvas.setTextColor(WHITE);
    canvas.drawRoundRect(5, 5, 230, 125, 7, WHITE);
    canvas.drawFastHLine(5, 20, 230, WHITE);
    canvas.drawString("PandoraOs 1.0.1", 10, 9);
    canvas.drawRightString(AppManager::getInstance().getCurrentAppName(), 230, 9);
    canvas.pushSprite(0, 0);  // Desenha o sprite na tela
}

M5Canvas& ScreenManager::getCanvas() {
    return canvas;
}
