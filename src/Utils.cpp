#include "Utils.h"

#include "ScreenManager.h"

void Utils::initCanvas() {
    canvas.setTextSize(1);
    canvas.setTextColor(WHITE);
    canvas.setTextWrap(true);
    canvas.fillScreen(BLACK);
    canvas.setCursor(0, 0);
    // reset font
    canvas.setFont(&Font0);
}