#ifndef GLOBAL_DISPLAY_H
#define GLOBAL_DISPLAY_H

#include <M5GFX.h>
#include <freertos/semphr.h>

struct window {
    int x0;
    int y0;
    int x1;
    int y1;
    int width;
    int height;
};

class GlobalDisplay {
   public:
    static GlobalDisplay& getInstance();

    M5GFX display;
    M5Canvas canvas;
    M5Canvas titleBar;
    M5Canvas center;
    M5Canvas footBar;
    SemaphoreHandle_t canvasSemaphore;

    M5Canvas* getCanvas() { return &canvas; }
    SemaphoreHandle_t getSemaphore() { return canvasSemaphore; }

    int titleBarHeight;
    int footBarHeight;
    int borderColor;
    window canvasStruct, titleBarStruct, centerStruct, footBarStruct;

    void footBarControl(bool show);
    void footBarControl(bool show, int height);

   private:
    GlobalDisplay();
    ~GlobalDisplay();

    int marginSize;
    int borderSize;
    int borderRadius;
    int zoom;

    static const char* TAG;

    // Delete copy constructor and assignment operator to enforce singleton pattern
    GlobalDisplay(const GlobalDisplay&) = delete;
    GlobalDisplay& operator=(const GlobalDisplay&) = delete;
};

#endif  // GLOBAL_DISPLAY_H
