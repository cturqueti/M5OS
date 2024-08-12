#include "Service.h"

#include "ScreenManager.h"

Service::Service() : taskHandle(nullptr) {}

Service::~Service() {
    // Se não houver nada específico a ser feito no destrutor, você pode deixá-lo vazio
}
void Service::begin() {
    // Inicializa qualquer configuração necessária para o serviço
    // M5Canvas& canvas = ScreenManager::getCanvas();
    // canvas.createSprite(240, 135);  // Ou o tamanho que você precisar
    // canvas.fillSprite(TFT_BLACK);   // Limpa o canvas
}

TaskHandle_t Service::getTaskHandle() const {
    return taskHandle;
}

void Service::setTaskHandle(TaskHandle_t handle) {
    taskHandle = handle;
}
