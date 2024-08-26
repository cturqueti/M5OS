#include "GlobalDisplay.h"

GlobalDisplay& GlobalDisplay::getInstance() {
    static GlobalDisplay instance;
    return instance;
}

GlobalDisplay::GlobalDisplay()
    : canvasSemaphore(xSemaphoreCreateMutex()),
      display(),
      canvas(&display),
      titleBar(&display),
      center(&display),
      titleBarHeight(16),
      marginSize(2),
      borderSize(1),
      borderRadius(10),
      borderColor(TFT_WHITE),
      zoom(1) {
    display.begin();
    if (display.isEPD()) {
        display.setEpdMode(epd_mode_t::epd_fastest);
        display.invertDisplay(true);
        display.clear(TFT_BLACK);
    }
    if (display.width() < display.height()) {
        display.setRotation(display.getRotation() ^ 1);
    }
    if (display.getBoard() == m5gfx::boards::board_t::board_M5StackCoreInk) {
        zoom = 2;
    } else if (display.getBoard() == m5gfx::boards::board_t::board_M5Paper) {
        zoom = 3;
    } else {
        zoom = (display.width() / 480) + 1;
    }

    canvasStruct.x0 = 0;
    canvasStruct.y0 = 0;
    canvasStruct.x1 = display.width() / zoom;
    canvasStruct.y1 = display.height() / zoom;
    canvasStruct.width = canvasStruct.x1 - canvasStruct.x0;
    canvasStruct.height = canvasStruct.y1 - canvasStruct.y0;
    canvas.createSprite(canvasStruct.width, canvasStruct.height);

    titleBarStruct.x0 = marginSize;
    titleBarStruct.y0 = marginSize;
    titleBarStruct.x1 = canvasStruct.x1 - marginSize;
    titleBarStruct.y1 = marginSize + 2 * borderSize + titleBarHeight;
    titleBarStruct.width = titleBarStruct.x1 - titleBarStruct.x0;
    titleBarStruct.height = titleBarStruct.y1 - titleBarStruct.y0;

    centerStruct.x0 = marginSize;
    centerStruct.y0 = titleBarStruct.y1;
    centerStruct.x1 = canvasStruct.x1 - marginSize;
    centerStruct.y1 = canvasStruct.y1 - marginSize;
    centerStruct.width = centerStruct.x1 - centerStruct.x0;
    centerStruct.height = centerStruct.y1 - centerStruct.y0;
}

GlobalDisplay::~GlobalDisplay() {
    if (canvasSemaphore != nullptr) {
        vSemaphoreDelete(canvasSemaphore);
    }
}
