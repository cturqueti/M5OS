#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Arduino.h>
#include <M5Cardputer.h>

#include "App.h"
#include "Globals.h"
#include "ScreenManager.h"

#define KEY_UP ';'
#define KEY_DOWN '.'
#define KEY_LEFT ','
#define KEY_RIGHT '/'

class Keyboard : public App {
   public:
    Keyboard();
    ~Keyboard();
    virtual void onAppOpen() override;
    virtual void onAppClose() override;
    virtual void onAppTick() override;
    virtual void draw() override;

   private:
    void handleInput();
    void drawText();

    M5Canvas tempCanvas;
    String inputText;          // Texto digitado
    const int maxLength = 20;  // Comprimento máximo do texto
    static Keyboard_Class::KeysState statusHold;
};

#endif  // KEYBOARD_H
