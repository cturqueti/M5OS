#include "App.h"

#include "ScreenManager.h"

App::App() : taskHandle(nullptr) {}

App::~App() {
}

void App::begin() {
    canvas.createSprite(240, 135);  // Ou o tamanho que você precisar
    canvas.fillSprite(TFT_BLACK);   // Limpa o canvas
}

TaskHandle_t App::getTaskHandle() const {
    return taskHandle;
}

void App::setTaskHandle(TaskHandle_t handle) {
    taskHandle = handle;
}

int App::getIconSize() const {
    // Retorne o tamanho do ícone. Substitua o valor por um valor apropriado.
    return 0;  // ou um valor adequado
}

const uint8_t* App::getIcon() const {
    // Retorne um ponteiro para os dados do ícone. Substitua com os dados apropriados.
    return nullptr;  // ou um ponteiro para os dados do ícone
}
