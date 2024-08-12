#include "Keyboard.h"

int Keyboard::ctrlBtn = 0;
bool Keyboard::showInputFlag = false;  // Inicialize os membros estáticos
bool Keyboard::isPressedHold = false;
String Keyboard::currentInput = "";
Keyboard_Class::KeysState Keyboard::statusHold;

Keyboard::Keyboard() : inputText(""), tempCanvas(&M5.Lcd) {
    tempCanvas.createSprite(240, 135);
}

Keyboard& Keyboard::getInstance() {
    static Keyboard instance;
    return instance;
}

Keyboard::~Keyboard() {
    tempCanvas.deleteSprite();
}

void Keyboard::onServiceOpen() {
    tempCanvas.createSprite(30, 13);
    // tempCanvas.fillSprite(TFT_GREEN);
}

void Keyboard::onServiceClose() {
    // Limpa a tela ao fechar
    // M5Cardputer.Display.clear();
}

void Keyboard::onServiceTick() {
    // Lida com a entrada do usuário
    // printf("KeyboardLigado");
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isPressed()) {
        printf("Pressionado algo\n");
        // handleInput();
    }
}

void Keyboard::draw() {
    // tempCanvas.fillSprite(TFT_GREEN);
    tempCanvas.fillCircle(7, 7, 5, TFT_ORANGE);
    drawText();
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        // Copia o conteúdo do canvas temporário para o canvas definitivo
        tempCanvas.pushSprite(&canvas, 120, 7);

        // Libera o semáforo após o uso
        xSemaphoreGive(canvasSemaphore);
        canvas.pushSprite(0, 0);
    }
}

bool Keyboard::shouldProcessKey() {
    return (millis() - lastKeyPressMillis >= debounceDelay);
}

void Keyboard::handleInput() {
    if (M5Cardputer.Keyboard.isChange()) {
        printf("Pressionado algo");
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (status.fn) {
            statusHold.fn = !statusHold.fn;
            StatusBar::draw();
        } else if (status.shift) {
            statusHold.shift = !statusHold.shift;
            StatusBar::draw();
        } else if (status.opt) {
            statusHold.opt = !statusHold.opt;
            StatusBar::draw();
        } else if (status.del && currentInput.length() > 0) {
            currentInput.remove(currentInput.length() - 1);
            lastKeyPressMillis = millis();
        } else if (status.enter) {
            input = currentInput;
            currentInput.clear();
        } else {
            if (shouldProcessKey()) {
                for (auto i : status.word) {
                    currentInput += i;
                }
                lastKeyPressMillis = millis();
            }
        }
    } else if (M5.BtnA.isPressed()) {
        exitBtn = true;
        printf("Pressionado exit");
    }
}

void Keyboard::drawText() {
    if (showInputFlag) {
    }
    // M5Cardputer.Display.clear();
    // M5Cardputer.Display.setCursor(0, 0);
    // M5Cardputer.Display.print("Digite: ");
    // M5Cardputer.Display.println(inputText);
}

void Keyboard::showInput(bool enable) {
    showInputFlag = enable;
}

const uint8_t* Keyboard::getIcon() {
    if (statusHold.fn) {
        return keyboardFnIcon;
    } else if (statusHold.shift) {
        return keyboardShiftIcon;
    } else if (statusHold.opt) {
        return keyboardOptIcon;
    } else {
        return keyboardDefaultIcon;
    }
}

size_t Keyboard::getIconSize() {
    if (statusHold.fn) {
        return sizeof(keyboardFnIcon);
    } else if (statusHold.shift) {
        return sizeof(keyboardShiftIcon);
    } else if (statusHold.opt) {
        return sizeof(keyboardOptIcon);
    } else {
        return sizeof(keyboardDefaultIcon);
    }
}

bool Keyboard::haveInput() {
    return haveInputFlag;
}

String Keyboard::getInput(bool clearInput) {
    String i = input;
    if (clearInput) {
        input.clear();
    }
    return i;
}

String Keyboard::getCurrentInput() {
    return currentInput;
}