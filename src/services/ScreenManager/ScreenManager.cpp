#include "ScreenManager.h"

#include "esp_log.h"

const char* ScreenManager::TAG = "ScreenManager";

bool ScreenManager::connected = false;
bool ScreenManager::disconnected = false;

ScreenManager::ScreenManager()
    : display(),
      priority(99),
      lastMillis(0),
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

    canvasX0 = 0;
    canvasY0 = 0;
    canvasX1 = display.width() / zoom;
    canvasY1 = display.height() / zoom;
    int canvasWidth = canvasX1 - canvasX0;
    int canvasHeight = canvasY1 - canvasY0;
    canvas.createSprite(canvasWidth, canvasHeight);

    int titleBarX0 = marginSize;
    int titleBarY0 = marginSize;
    int titleBarX1 = canvasX1 - marginSize;
    int titleBarY1 = marginSize + 2 * borderSize + titleBarHeight;
    titleBar.begin(titleBarX0, titleBarY0, titleBarX1, titleBarY1);
    titleBar.configColors(TFT_BLACK, TFT_GREEN, TFT_WHITE);

    int centerX0 = marginSize;
    int centerY0 = titleBarY1;
    int centerX1 = canvasX1 - marginSize;
    int centerY1 = canvasY1 - marginSize;
    center.begin(centerX0, centerY0, centerX1, centerY1);
    center.configColors(TFT_BLACK, TFT_GREEN, TFT_WHITE);
}

ScreenManager::~ScreenManager() {
    canvas.deleteSprite();
}

void ScreenManager::onServiceOpen() {
    titleBar.setText("Texto no TitleBar");
    center.setText(textVector);

    titleBar.draw();
    center.draw();

    canvas.pushImage(titleBar.getX(), titleBar.getY(), titleBar.getWidth(), titleBar.getHeight(), titleBar.getBuffer());
    canvas.pushImage(center.getX(), center.getY(), center.getWidth(), center.getHeight(), center.getBuffer());

    canvas.pushSprite(0, 0);
}

void ScreenManager::onServiceTick() {
    if (millis() - lastMillis > 100) {
        lastMillis = millis();
    }
}

void ScreenManager::onServiceClose() {
    ESP_LOGI(TAG, "Fechando a tela.");
}

void ScreenManager::draw() {}

const uint8_t* ScreenManager::getIcon() {
    return 0;
}

size_t ScreenManager::getIconSize() {
    return 0;
}
