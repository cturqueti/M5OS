#include "App.h"

App::App() : taskHandle(nullptr) {}

App::~App() {
    // Destruição de recursos, se necessário
}

TaskHandle_t App::getTaskHandle() const {
    return taskHandle;
}

void App::setTaskHandle(TaskHandle_t handle) {
    taskHandle = handle;
}
