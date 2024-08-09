#include "App.h"

#include "ScreenManager.h"  // Assumindo que o ScreenManager tem a definição do canvas

App::App() : taskHandle(nullptr) {}

App::~App() {
    // Se não houver nada específico a ser feito no destrutor, você pode deixá-lo vazio
}

void App::begin() {
    // Supondo que canvas é um membro estático ou singleton do ScreenManager
    M5Canvas& canvas = ScreenManager::getCanvas();
    canvas.createSprite(240, 135);  // Ou o tamanho que você precisar
    canvas.fillSprite(TFT_BLACK);   // Limpa o canvas
}

TaskHandle_t App::getTaskHandle() const {
    return taskHandle;
}

void App::setTaskHandle(TaskHandle_t handle) {
    taskHandle = handle;
}
