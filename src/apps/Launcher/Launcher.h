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
    M5Canvas canvas;
    static const char* TAG;
    uint16_t bgColor, textColor, borderColor;
    long int lastMillis;
    int selectIndex;
    bool needRedraw;
    std::vector<String> apps;
    window* centerSizes;

    int32_t centerX0 = static_cast<int32_t>(centerSizes->x0);
    int32_t centerY0 = static_cast<int32_t>(centerSizes->y0);  // or wherever y0 is set
    int32_t centerX1 = static_cast<int32_t>(centerSizes->x1);
    int32_t centerY1 = static_cast<int32_t>(centerSizes->y1);
    int32_t centerWidth = static_cast<int32_t>(centerSizes->width);
    int32_t centerHeight = static_cast<int32_t>(centerSizes->height);
    void clearCenter();
};

#endif  // LAUNCHER_H
