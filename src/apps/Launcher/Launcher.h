#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <WString.h>

#include <vector>

#include "../../services/ScreenManager/ScreenManager.h"
#include "../assets/m5os.h"
#include "App.h"
#include "AppManager.h"
#include "GlobalDisplay.h"
#include "Globals.h"
#include "SD.h"
#include "Utils.h"

class Launcher : public App {
   public:
    Launcher();
    ~Launcher();
    void onAppOpen() override;
    void onAppClose() override;
    void onAppTick() override;
    void draw() override;

   private:
    uint16_t nloop;
    M5Canvas center;
    static const char* TAG;
    int x0, y0, x1, y1, width, height;
    uint16_t bgColor, textColor, borderColor;
    long int lastMillis;
    int selectIndex;
    bool needRedraw;
    std::vector<String> apps;
};

#endif  // LAUNCHER_H
