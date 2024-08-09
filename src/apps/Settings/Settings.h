#ifndef SETTINGS_H
#define SETTINGS_H

#include <WString.h>

#include <vector>

#include "App.h"
#include "AppManager.h"
#include "Globals.h"
#include "M5Cardputer.h"
#include "M5GFX.h"
#include "SD.h"
#include "ScreenManager.h"
#include "Utils.h"
#include "icon.h"

class Settings : public App {
   public:
    Settings();
    virtual ~Settings();

    void onAppOpen() override;
    void onAppClose() override;
    void onAppTick() override;
    void draw() override;
    
    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   private:
};

#endif  // SETTINGS_H
