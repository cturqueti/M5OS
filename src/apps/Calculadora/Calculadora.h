#ifndef CALCULADORA_H
#define CALCULADORA_H

#include <WString.h>

#include <vector>

#include "App.h"
#include "AppManager.h"
#include "Globals.h"
#include "SD.h"
#include "ScreenManager.h"
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

    int appPriority() const override { return 2; }

    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   private:
    uint16_t nloop;
};

#endif  // CALCULADORA_H
