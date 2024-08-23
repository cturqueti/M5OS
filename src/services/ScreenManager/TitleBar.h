#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <Arduino.h>
#include <M5GFX.h>

class TitleBar {
   public:
    TitleBar(M5GFX* display);
    ~TitleBar();

    void begin(int x0, int y0, int x1, int y1);
    void configColors(int bgColor, int textColor, int borderColor);
    void draw();
    inline void setText(const char* text) { this->text = text; }

    inline int getX() const { return x0; }
    inline int getY() const { return y0; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline const uint16_t* getBuffer() const { return static_cast<const uint16_t*>(canvas.getBuffer()); }

   private:
    M5GFX* display;
    M5Canvas canvas;
    const char* text;
    int x0, y0, x1, y1, width, height;
    uint16_t bgColor, textColor, borderColor;
};

#endif  // TITLEBAR_H
