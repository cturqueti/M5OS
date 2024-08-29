#ifndef CALCULADORA_H
#define CALCULADORA_H

// #include <Adafruit_GFX.h>
#include <WString.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <vector>

#include "../../services/ScreenManager/ScreenManager.h"
#include "App.h"
#include "AppManager.h"
#include "Globals.h"
#include "SD.h"
#include "Utils.h"
#include "icon.h"

class Calculadora : public App {
   public:
    Calculadora();
    ~Calculadora();
    void onAppOpen() override;
    void onAppClose() override;
    void onAppTick() override;
    void draw() override;

    inline int appPriority() const override { return priority; }

    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   private:
    uint8_t priority;
    uint16_t nloop;
    M5Canvas canvas;
    uint16_t bgColor, textColor, borderColor;

    window* centerSizes;
    window* footBarSizes;

    int32_t centerX0 = static_cast<int32_t>(centerSizes->x0);
    int32_t centerY0 = static_cast<int32_t>(centerSizes->y0);  // or wherever y0 is set
    int32_t centerX1 = static_cast<int32_t>(centerSizes->x1);
    int32_t centerY1 = static_cast<int32_t>(centerSizes->y1);
    int32_t centerWidth = static_cast<int32_t>(centerSizes->width);
    int32_t centerHeight = static_cast<int32_t>(centerSizes->height);
    int32_t footBarX0 = static_cast<int32_t>(footBarSizes->x0);
    int32_t footBarY0 = static_cast<int32_t>(footBarSizes->y0);  // or wherever y0 is set
    int32_t footBarX1 = static_cast<int32_t>(footBarSizes->x1);
    int32_t footBarY1 = static_cast<int32_t>(footBarSizes->y1);
    int32_t footBarWidth = static_cast<int32_t>(footBarSizes->width);
    int32_t footBarHeight = static_cast<int32_t>(footBarSizes->height);

    void clearCenter();
    void clearFootBar();
};

#endif  // CALCULADORA_H
