#ifndef CENTER_H
#define CENTER_H

#include <Arduino.h>
#include <M5GFX.h>

#include <map>
#include <string>

class Center {
   public:
    Center(M5GFX* display);
    ~Center();

    void begin(int x0, int y0, int x1, int y1);
    void configColors(int bgColor, int textColor, int borderColor);
    void draw();
    void setText(const std::vector<std::pair<std::string, std::string>>& textVector);

    inline int getX() const { return x0; }
    inline int getY() const { return y0; }
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline const uint16_t* getBuffer() const { return static_cast<const uint16_t*>(canvas.getBuffer()); }

   private:
    M5GFX* display;
    M5Canvas canvas;
    std::vector<std::pair<std::string, std::string>> textVector;
    int x0, y0, width, height;
    uint16_t bgColor, textColor, borderColor, textParagraph;
    int lineHeight;  // Altura da linha para o texto
    int maxLines;    // Número máximo de linhas a serem exibidas
    static const char* TAG;

    int getTextWidth(const std::string& text);
    std::vector<std::string> wrapText(const std::string& text, int maxWidth);
};

#endif  // CENTER_H
