#include "Keyboard.h"

int Keyboard::ctrlBtn = 0;
bool Keyboard::showInputFlag = false;  // Inicialize os membros estáticos
bool Keyboard::isPressedHold = false;
String Keyboard::currentInput = "";
String Keyboard::command = "";
String Keyboard::_lastCommand = "";
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
    M5Cardputer.update();

    if (M5Cardputer.Keyboard.isChange()) {
        handleInput();
    }
    if (M5.BtnA.isPressed()) {
        if (shouldProcessKey()) {
            exitBtn = true;
            command = "EXIT";
            // printf("Pressionado exit\n");
            lastKeyPressMillis = millis();
        }
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
    if (M5Cardputer.Keyboard.isPressed()) {
        // printf("Pressionado algo\n");
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (status.fn) {
            statusHold.fn = !statusHold.fn;
            printf("Fn: %d\n", statusHold.fn);
            StatusBar::draw();
        } else if (status.shift) {
            statusHold.shift = !statusHold.shift;
            M5Cardputer.Keyboard.setCapsLocked(statusHold.shift);
            printf("Shift: %d\n", statusHold.shift);
            StatusBar::draw();
        } else if (status.opt) {
            printf("OPT: %d\n", statusHold.opt);
            statusHold.opt = !statusHold.opt;
            StatusBar::draw();
        } else if (status.del && currentInput.length() > 0) {
            if (shouldProcessKey()) {
                lastKeyPressMillis = millis();
                currentInput.remove(currentInput.length() - 1);

                printf("Entrada atual: %s\n", currentInput.c_str());
            }
        } else if (status.enter) {
            if (shouldProcessKey()) {
                input = currentInput;
                currentInput.clear();
                command = "ENTER";

                lastKeyPressMillis = millis();
            }
        } else if (status.ctrl) {
            command.clear();
            if (shouldProcessKey()) {
                for (auto i : status.word) {
                    command = i;
                }

                lastKeyPressMillis = millis();
                printf("Ctrl + %s\n", command.c_str());
            }
        } else if (status.tab) {
            // Calcula quantos espaços adicionar para alinhar a 4 caracteres
            int spacesToAdd = 4 - (currentInput.length() % 4);
            for (int i = 0; i < spacesToAdd; i++) {
                currentInput += ' ';
            }
            lastKeyPressMillis = millis();
        } else if (statusHold.fn) {
            if (shouldProcessKey()) {
                for (auto i : status.word) {
                    if (i == KEY_UP) {
                        command = "UP";
                    } else if (i == KEY_DOWN) {
                        command = "DOWN";
                    } else if (i == KEY_LEFT) {
                        command = "LEFT";
                    } else if (i == KEY_RIGHT) {
                        command = "RIGHT";
                    } else if (i == KEY_ESC) {
                        command = "ESC";
                    }
                }
                printf("KEY: %s\n", command.c_str());
                lastKeyPressMillis = millis();
            }
        } else {
            if (shouldProcessKey()) {
                lastKeyPressMillis = millis();
                for (auto i : status.word) {
                    currentInput += i;
                }
            }
            printf("Entrada atual: %s\n", currentInput.c_str());
        }
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

bool Keyboard::commandIsChange() {
    if (command.length() > 0) {
        // printf("Verificado se há comando");
        return true;
    } else {
        return false;
    }
}

String Keyboard::getCommand() {
    String i = command;
    printf("Efetuado a leitura do comando: %s\n", i);
    command.clear();
    return i;
}
