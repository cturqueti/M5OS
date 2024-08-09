#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#pragma once

#include "Globals.h"
#include "M5Cardputer.h"
#include "M5GFX.h"

extern M5Canvas canvas;

struct DrawingArea {
    int xStart;  // Posição x inicial para desenho
    int yStart;  // Posição y inicial para desenho
    int width;   // Largura útil
    int height;  // Altura útil
};

class StatusBar {
   private:
    static M5Canvas tempCanvas;

   public:
    StatusBar();
    ~StatusBar();
    static void draw(bool force = false);
    static DrawingArea area;
};

class ScreenManager {
   public:
    static M5Canvas& getCanvas();  // Retorna uma referência ao canvas
};

#endif  // SCREENMANAGER_H
