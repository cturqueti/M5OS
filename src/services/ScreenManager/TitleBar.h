#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <Arduino.h>
#include <M5GFX.h>

#include "AppManager.h"
#include "GlobalDisplay.h"
#include "Globals.h"
#include "Service.h"

class TitleBar : public Service {
   public:
    TitleBar();
    ~TitleBar();

    void onServiceOpen() override;
    void onServiceTick() override;
    void onServiceClose() override;
    void draw() override;
    const uint8_t* getIcon() { return nullptr; }
    size_t getIconSize() { return 0; }

    void configColors(int bgColor, int textColor, int borderColor);

    inline int getX() const { return titleBarX0; }
    inline int getY() const { return titleBarY0; }
    inline int getWidth() const { return titleBarWidth; }
    inline int getHeight() const { return titleBarHeight; }

   private:
    M5Canvas titleBar;
    static const char* TAG;

    uint16_t bgColor, textColor, borderColor;
    long int lastMillis;
    window* titleBarSizes;

    int32_t titleBarX0 = static_cast<int32_t>(titleBarSizes->x0);
    int32_t titleBarY0 = static_cast<int32_t>(titleBarSizes->y0);  // or wherever y0 is set
    int32_t titleBarX1 = static_cast<int32_t>(titleBarSizes->x1);
    int32_t titleBarY1 = static_cast<int32_t>(titleBarSizes->y1);
    int32_t titleBarWidth = static_cast<int32_t>(titleBarSizes->width);
    int32_t titleBarHeight = static_cast<int32_t>(titleBarSizes->height);
    void clearTitleBar();
};

#endif  // TITLEBAR_H