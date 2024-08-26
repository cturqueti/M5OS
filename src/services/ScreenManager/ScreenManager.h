#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <Arduino.h>
#include <M5GFX.h>
#include <Preferences.h>
#include <SPI.h>
#include <SPIFFS.h>

#include <map>
#include <string>
#include <vector>

#include "AppManager.h"
#include "Globals.h"
#include "Service.h"

#define LINE_COUNT 6

// class Center;

// class ScreenManager : public Service {
//    public:
//     static ScreenManager& getInstance() {
//         static ScreenManager instance;
//         return instance;
//     }

//     ScreenManager();
//     ~ScreenManager();

//     void onServiceOpen();
//     void onServiceTick();
//     void onServiceClose();
//     void draw();
//     const uint8_t* getIcon();
//     size_t getIconSize();

//     int titleBarHeight;
//     int borderColor;

//    private:
//     // GFXcanvas16 canvas;

//     // ----------- estancias -----------//
//     Preferences preferences;

//     // ------- variávels padrão ------- //
//     static bool connected;
//     static bool disconnected;
//     int lastMillis;
//     uint8_t priority;
//     static const char* TAG;
//     // ------ constantes da tela ------ //
//     int marginSize;
//     int borderSize;
//     int borderRadius;
//     int zoom;

//     int canvasX0, canvasX1, canvasY0, canvasY1;

//     std::vector<std::pair<std::string, std::string>> textVector = {
//         {"Line 1", "This is the first line."},
//         {"Line 2", "This is the second line."},
//         {"Line 3", "This is the third line."},
//         {"Line 4", "This is the fourth line."},
//         {"Line 5", "This is the fifth line."},
//         {"Line 6", "This is the sixth line."},
//         {"Line 7", "This is the seventh line."},
//         {"Line 8", "This is the eighth line."},
//         {"Line 9", "This is the ninth line."},
//         {"Line 10", "This is the tenth line."},
//         {"Line 11", "This is the eleventh line."},
//         {"Line 12", "This is the twelfth line."},
//         {"Line 13", "This is the thirteenth line."},
//         {"Line 14", "This is the fourteenth line."},
//         {"MQTT", "This is the fifteenth line."},
//         {"16", "This is the sixteenth line."},
//         {"Line 17", "This is the seventeenth line, teste width."},
//         {"Line 18", "This is the eighteenth line."},
//         {"Line 19", "This is the nineteenth line."},
//         {"Line 20", "This is the twentieth line."}};
// };

// class Center {
//    public:
//     Center(M5GFX* display);
//     ~Center();

//     void begin(int x0, int y0, int x1, int y1);
//     void configColors(int bgColor, int textColor, int borderColor);
//     void draw();
//     void setText(const std::vector<std::pair<std::string, std::string>>& textVector);

//     inline int getX() const { return x0; }
//     inline int getY() const { return y0; }
//     inline int getWidth() const { return width; }
//     inline int getHeight() const { return height; }
//     inline const uint16_t* getBuffer() const { return static_cast<const uint16_t*>(canvas.getBuffer()); }

//    private:
//     std::vector<std::pair<std::string, std::string>> textVector;
//     int x0, y0, width, height;
//     uint16_t bgColor, textColor, borderColor, textParagraph;
//     int lineHeight;  // Altura da linha para o texto
//     int maxLines;    // Número máximo de linhas a serem exibidas
//     static const char* TAG;

//     int getTextWidth(const std::string& text);
//     std::vector<std::string> wrapText(const std::string& text, int maxWidth);
// };

#endif  // SCREENMANAGER_H
