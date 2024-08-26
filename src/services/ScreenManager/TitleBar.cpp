#include "TitleBar.h"

#include "esp_log.h"

const char* TitleBar::TAG = "TitleBar";

TitleBar::TitleBar()
    : x0(GlobalDisplay::getInstance().titleBarStruct.x0),
      y0(GlobalDisplay::getInstance().titleBarStruct.y0),
      x1(GlobalDisplay::getInstance().titleBarStruct.x1),
      y1(GlobalDisplay::getInstance().titleBarStruct.y1),
      width(GlobalDisplay::getInstance().titleBarStruct.width),
      height(GlobalDisplay::getInstance().titleBarStruct.height),
      titleBar(*GlobalDisplay::getInstance().getCanvas()),
      lastMillis(millis()),
      bgColor(TFT_BLACK),
      textColor(TFT_WHITE),
      borderColor(TFT_WHITE) {
}

TitleBar::~TitleBar() {
}

void TitleBar::onServiceOpen() {
    draw();
}

void TitleBar::onServiceTick() {
    if (millis() - lastMillis > 200) {
        lastMillis = millis();
        draw();
    }
}

void TitleBar::onServiceClose() {}

void TitleBar::draw() {
    // Define border parameters
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    int radius = 10;  // Radius for the rounded corners
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw the top horizontal line with rounded corners
        titleBar.drawRect(x0, y0, width, height, bgColor);
        titleBar.drawLine(x0 + radius, y0, x0 + width - radius - 1, y0, borderColor);

        // Draw the vertical sides
        titleBar.drawLine(x0, y0 + radius, x0, y0 + height - 1, borderColor);                          // Left side
        titleBar.drawLine(x0 + width - 1, y0 + radius, x0 + width - 1, y0 + height - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        titleBar.drawLine(x0, y0 + height - 1, x0 + width - 1, y0 + height - 1, borderColor);

        // Draw the top-left rounded corner
        titleBar.drawCircleHelper(x0 + radius, y0 + radius, radius, 1, borderColor);

        // Draw the top-right rounded corner
        titleBar.drawCircleHelper(x0 + width - radius - 1, y0 + radius, radius, 2, borderColor);

        // Draw the text
        titleBar.setTextSize(1);
        titleBar.setCursor(x0 + 5, y0 + (height - 1) / 2);
        titleBar.print(AppManager::getInstance().getCurrentAppName().c_str());
        titleBar.setTextColor(textColor);

        titleBar.pushSprite(0, 0);
        xSemaphoreGive(canvasSemaphore);
        // ESP_LOGI(TAG, "Imprimiu");
    }
}

void TitleBar::configColors(int bgColor, int textColor, int borderColor) {
    this->bgColor = bgColor;
    this->textColor = textColor;
    this->borderColor = borderColor;
}