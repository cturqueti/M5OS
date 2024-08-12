#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include <M5Cardputer.h>
#include <WString.h>

#include <vector>

#include "Globals.h"
#include "KeyboardDefault.h"
#include "KeyboardFn.h"
#include "KeyboardOpt.h"
#include "KeyboardShift.h"
#include "M5Cardputer.h"
#include "M5GFX.h"
#include "ScreenManager.h"
#include "Service.h"
#include "ServicesManager.h"
#include "Utils.h"
#include "services/Wifi/WifiConnected.h"

#define KEY_UP ';'
#define KEY_DOWN '.'
#define KEY_LEFT ','
#define KEY_RIGHT '/'

class Keyboard : public Service {
   public:
    Keyboard();
    virtual ~Keyboard();
    static Keyboard& getInstance();
    virtual void onServiceOpen() override;
    virtual void onServiceClose() override;
    virtual void onServiceTick() override;
    virtual void draw() override;
    void showInput(bool enable);
    String input = "";

    const uint8_t* getIcon() override;
    size_t getIconSize() override;

    bool haveInput();
    String getInput(bool clearInput);
    String getCurrentInput();

    bool exitBtn = false;

    static int ctrlBtn;

   private:
    Keyboard(const Keyboard&) = delete;
    void operator=(const Keyboard&) = delete;

    void handleInput();
    void drawText();
    bool shouldProcessKey();

    M5Canvas tempCanvas;
    String inputText;          // Texto digitado
    const int maxLength = 20;  // Comprimento máximo do texto
    // static Keyboard_Class::KeysState statusHold;
    static bool showInputFlag;
    static Keyboard_Class::KeysState statusHold;
    static bool isPressedHold;
    static String currentInput;
    unsigned long lastKeyPressMillis = 0;
    const unsigned long debounceDelay = 130;
    bool haveInputFlag = false;
};

#endif  // KEYBOARD_H
