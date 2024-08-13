#include "Launcher.h"

M5Canvas Launcher::tempCanvas(&M5.Lcd);
DrawingArea Launcher::drawingArea;

Launcher::Launcher() : selectIndex(0), needRedraw(true) {
    drawingArea.width = 240;
    drawingArea.height = 118;
    drawingArea.xStart = 0;
    drawingArea.yStart = 17;
    tempCanvas.createSprite(240, 118);
}

Launcher::~Launcher() {
    tempCanvas.deleteSprite();
}

void Launcher::onAppOpen() {
    // StatusBar::draw();
    // drawingArea = StatusBar::area;
    // tempCanvas.createSprite(drawingArea.width, drawingArea.height);
    // Obtém todos os aplicativos do AppManager
    auto fetchedApps = AppManager::getInstance().listApps();
    for (auto& app : fetchedApps) {
        if (app.first == "Launcher") {
            continue;
        }
        apps.emplace_back(app.first.c_str());
    }
    selectIndex = 0;
    needRedraw = true;
}

void Launcher::onAppClose() {
    apps.clear();
}

void Launcher::onAppTick() {
    if (needRedraw) {
        // Utils::initCanvas();
        // StatusBar::draw(true);
        tempCanvas.createSprite(drawingArea.width, drawingArea.height);
        tempCanvas.fillSprite(TFT_BLACK);
        if (apps.empty()) {
            tempCanvas.setTextSize(2);
            tempCanvas.setTextColor(WHITE);
            tempCanvas.drawCenterString("No apps found", 120, 66);
            tempCanvas.pushSprite(0, 0);
            needRedraw = false;
            return;
        }

        tempCanvas.setTextSize(2);
        tempCanvas.setTextColor(WHITE);
        tempCanvas.drawCenterString(apps[selectIndex], drawingArea.width / 2, 5);
        // printf("Programa selecionado %s\n", apps[selectIndex].c_str());
        if (apps.size() > 1) {
            tempCanvas.setCursor(10, drawingArea.height / 2);
            tempCanvas.print("<");
            tempCanvas.drawRightString(">", 230, drawingArea.height / 2);
        }
        int x = ((drawingArea.width - 75) / 2);
        int y2 = ((drawingArea.height - 75 + 10) / 2);

        if (AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize() != 0) {
            tempCanvas.drawPng(AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIcon(),
                               AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize(),
                               x, y2, 75, 75);
        } else {
            canvas.drawPng(m5os, sizeof(m5os), x, y2, 75, 75);
        }

        needRedraw = false;
    }
    if (Keyboard::getInstance().commandIsChange() && !apps.empty() && AppManager::getInstance().getCurrentAppName() == "Launcher") {
        String command = Keyboard::getInstance().getCommand();
        if (command == "LEFT") {
            selectIndex++;
            if (selectIndex >= apps.size()) {
                selectIndex = 0;
            }
            needRedraw = true;
        }
        if (command == "RIGHT") {
            selectIndex--;
            if (selectIndex < 0) {
                selectIndex = apps.size() - 1;
            }
            needRedraw = true;
        }
        if (command == "ENTER") {
            AppManager::getInstance().openApp(apps[selectIndex].c_str());
            // AppManager::getInstance().closeApp("Launcher");
        }
    }
}

void Launcher::draw() {
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        tempCanvas.pushSprite(&canvas, drawingArea.xStart, drawingArea.yStart);

        xSemaphoreGive(canvasSemaphore);
    }
}
