#ifndef CALCULADORA_H
#define CALCULADORA_H

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

class Calculadora : public App {
   private:
    M5Canvas tempCanvas;

   public:
    Calculadora();
    virtual ~Calculadora();

    void onAppOpen() override;
    void onAppClose() override;
    void onAppTick() override;
    void draw() override;

    const uint8_t* getIcon() override;
    size_t getIconSize() override;
};

#endif  // CALCULADORA_H
