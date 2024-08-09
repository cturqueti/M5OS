#include "Launcher.h"

M5Canvas Launcher::tempCanvas(&M5.Lcd);

Launcher::Launcher() : selectIndex(0), needRedraw(true) {
    tempCanvas.createSprite(240, 135);
}

Launcher::~Launcher() {
    tempCanvas.deleteSprite();
}

void Launcher::onAppOpen() {
    StatusBar::draw();
    DrawingArea area = StatusBar::area;
    tempCanvas.createSprite(area.width, area.height);
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
        tempCanvas.createSprite(240, 135);
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
        tempCanvas.drawCenterString(apps[selectIndex], 120, 110);
        printf("Programa selecionado %s\n", apps[selectIndex].c_str());
        if (apps.size() > 1) {
            tempCanvas.setCursor(10, 67);
            tempCanvas.print("<");
            tempCanvas.drawRightString(">", 230, 67);
        }
        int x = ((240 - 75) / 2);
        int y2 = ((135 - 75) / 2);

        if (AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize() != 0) {
            tempCanvas.drawPng(AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIcon(),
                               AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize(),
                               x, y2, 75, 75);
        } else {
            // nop
        }

        needRedraw = false;
    }
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && !apps.empty()) {
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (M5Cardputer.Keyboard.isKeyPressed('/')) {
            selectIndex++;
            if (selectIndex >= apps.size()) {
                selectIndex = 0;
            }
            needRedraw = true;
        }
        if (M5Cardputer.Keyboard.isKeyPressed(',')) {
            selectIndex--;
            if (selectIndex < 0) {
                selectIndex = apps.size() - 1;
            }
            needRedraw = true;
        }
        if (status.enter) {
            AppManager::getInstance().openApp(apps[selectIndex].c_str());
        }
    }
}

void Launcher::draw() {
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        canvas.fillSprite(TFT_BLACK);

        // Copia o conteúdo do canvas temporário para o canvas definitivo
        tempCanvas.pushSprite(&canvas, 0, 0);

        // Libera o semáforo após o uso
        xSemaphoreGive(canvasSemaphore);
        canvas.pushSprite(0, 0);
    }
}
