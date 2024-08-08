#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#pragma once

#include "M5Cardputer.h"
#include "M5GFX.h"

extern M5Canvas canvas;

class StatusBar {
   public:
    static void draw(bool force = false);
};

class ScreenManager {
   public:
    static M5Canvas& getCanvas();  // Retorna uma referência ao canvas
};

#endif  // SCREENMANAGER_H
