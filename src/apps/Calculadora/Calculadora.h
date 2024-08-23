#ifndef CALCULADORA_H
#define CALCULADORA_H

// #include <Adafruit_GFX.h>
#include <WString.h>

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
};

#endif  // CALCULADORA_H
