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

    inline int getX() const { return x0; }
    inline int getY() const { return y0; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }

   private:
    M5Canvas titleBar;
    static const char* TAG;
    int x0, y0, x1, y1, width, height;
    uint16_t bgColor, textColor, borderColor;
    long int lastMillis;
};

#endif  // TITLEBAR_H