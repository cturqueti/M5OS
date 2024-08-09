#include "Launcher.h"

Launcher::Launcher() : selectIndex(0), needRedraw(true) {
    // Inicialização do Launcher
}

Launcher::~Launcher() {
    // Limpeza dos recursos
}

void Launcher::onAppOpen() {
    // Obtém todos os aplicativos do AppManager
    auto fetchedApps = AppManager::getInstance().listApps();
    for (auto &app : fetchedApps) {
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
        Utils::initCanvas();
        StatusBar::draw(true);
        if (apps.empty()) {
            canvas.setTextSize(2);
            canvas.setTextColor(WHITE);
            canvas.drawCenterString("No apps found", 120, 66);
            canvas.pushSprite(0, 0);
            needRedraw = false;
            return;
        }
        canvas.setTextSize(2);
        canvas.setTextColor(WHITE);
        canvas.drawCenterString(apps[selectIndex], 120, 110);
        if (apps.size() > 1) {
            canvas.setCursor(10, 67);
            canvas.print("<");
            canvas.drawRightString(">", 230, 67);
        }
        String iconPath = "/icons/" + apps[selectIndex] + ".png";
        int x = ((240 - 75) / 2);
        int y2 = ((135 - 75) / 2);

        if (AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize() != 0) {
            canvas.drawPng(AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIcon(),
                           AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize(),
                           x, y2, 75, 75);
        } else {
            // canvas.drawPng(hydra, 435, x + 5, y2 + 5, 75, 75, 0, 0, 2, 2);
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
    canvas.pushSprite(0, 0);
}
