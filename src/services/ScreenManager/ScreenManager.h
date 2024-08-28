#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Arduino.h>
#include <M5GFX.h>
#include <Preferences.h>
#include <SPI.h>
#include <SPIFFS.h>

#include <map>
#include <string>
#include <vector>

#include "AppManager.h"
#include "Globals.h"
#include "Service.h"

#define LINE_COUNT 6

// class Center;

class ScreenManager : public Service {
   public:
    ScreenManager();
    ~ScreenManager();

    void onServiceOpen();
    void onServiceTick();
    void onServiceClose();
    void draw();
    const uint8_t* getIcon();
    size_t getIconSize();

    int titleBarHeight;
    int borderColor;

   private:
    M5Canvas canvas;
    static const char* TAG;
    // ------ constantes da tela ------ //
};
#endif  // SCREENMANAGER_H