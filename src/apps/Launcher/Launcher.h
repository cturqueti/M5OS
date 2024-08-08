#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <WString.h>

#include <vector>

#include "App.h"
#include "AppManager.h"
#include "SD.h"
#include "ScreenManager.h"
#include "Utils.h"

class Launcher : public App {
   private:
    int selectIndex;
    bool needRedraw;
    std::vector<String> apps;

   public:
    Launcher();
    ~Launcher();
    void onAppOpen() override;
    void onAppClose() override;
    void onAppTick() override;

    void draw() override;
};

#endif  // LAUNCHER_H
