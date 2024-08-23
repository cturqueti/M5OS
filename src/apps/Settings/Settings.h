#ifndef SETTINGS_H
#define SETTINGS_H

#include <WString.h>

#include <vector>

#include "../../services/ScreenManager/ScreenManager.h"
#include "App.h"
#include "AppManager.h"
#include "Globals.h"
#include "SD.h"
#include "Utils.h"
#include "icon.h"

class Settings : public App {
   public:
    Settings();
    ~Settings();
    void onAppOpen() override;
    void onAppClose() override;
    void onAppTick() override;
    void draw() override;

    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   private:
    uint64_t delay_t = 1000;
    uint64_t lastMillis = 0;
};

#endif  // SETTINGS_H
