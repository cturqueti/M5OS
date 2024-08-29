#include "TitleBar.h"

#include "esp_log.h"

const char* TitleBar::TAG = "TitleBar";

TitleBar::TitleBar()
    : titleBarSizes(&GlobalDisplay::getInstance().titleBarStruct),
      titleBar(*GlobalDisplay::getInstance().getCanvas()),
      lastMillis(millis()),
      bgColor(TFT_BLACK),
      textColor(TFT_WHITE),
      borderColor(TFT_WHITE) {
}

TitleBar::~TitleBar() {
}

void TitleBar::onServiceOpen() {
    // draw();
}

void TitleBar::onServiceTick() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    // if (millis() - lastMillis > 200) {
    //     lastMillis = millis();
    //     draw();
    // }
    clearTitleBar();
    // Draw the text
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        titleBar.setTextSize(1);
        titleBar.setCursor(titleBarX0 + 5, titleBarY0 + (titleBarHeight - 1) / 2);
        titleBar.print(AppManager::getInstance().getCurrentAppName().c_str());
        titleBar.setTextColor(textColor);

        xSemaphoreGive(canvasSemaphore);
    }
}

void TitleBar::onServiceClose() {}

void TitleBar::draw() {
    // Define border parameters
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    int radius = 10;  // Radius for the rounded corners
    titleBarX0 = static_cast<int32_t>(titleBarSizes->x0);
    titleBarY0 = static_cast<int32_t>(titleBarSizes->y0);  // or wherever titleBarY0 is set
    titleBarX1 = static_cast<int32_t>(titleBarSizes->x1);
    titleBarY1 = static_cast<int32_t>(titleBarSizes->y1);
    titleBarWidth = static_cast<int32_t>(titleBarSizes->width);
    titleBarHeight = static_cast<int32_t>(titleBarSizes->height);

    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw the top horizontal line with rounded corners
        titleBar.drawLine(titleBarX0 + radius, titleBarY0, titleBarX0 + titleBarWidth - radius - 1, titleBarY0, borderColor);

        // Draw the vertical sides
        titleBar.drawLine(titleBarX0, titleBarY0 + radius, titleBarX0, titleBarY0 + titleBarHeight - 1, borderColor);                                          // Left side
        titleBar.drawLine(titleBarX0 + titleBarWidth - 1, titleBarY0 + radius, titleBarX0 + titleBarWidth - 1, titleBarY0 + titleBarHeight - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        // titleBar.drawLine(titleBarX0, titleBarY0 + titleBarHeight - 1, titleBarX0 + titleBarWidth - 1, titleBarY0 + titleBarHeight - 1, borderColor);

        // Draw the top-left rounded corner
        titleBar.drawCircleHelper(titleBarX0 + radius, titleBarY0 + radius, radius, 1, borderColor);

        // Draw the top-right rounded corner
        titleBar.drawCircleHelper(titleBarX0 + titleBarWidth - radius - 1, titleBarY0 + radius, radius, 2, borderColor);

        xSemaphoreGive(canvasSemaphore);
    }
}

void TitleBar::configColors(int bgColor, int textColor, int borderColor) {
    this->bgColor = bgColor;
    this->textColor = textColor;
    this->borderColor = borderColor;
}
void TitleBar::clearTitleBar() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        titleBar.fillRect(titleBarX0, titleBarY0, titleBarWidth, titleBarHeight, bgColor);
        xSemaphoreGive(canvasSemaphore);
    }
}