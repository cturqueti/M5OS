#include "App.h"

#include "ScreenManager.h"  // Assumindo que o ScreenManager tem a definição do canvas
#include "esp_log.h"
static const char* TAG = "App";

TaskHandle_t App::taskHandle = nullptr;

// Construtor
App::App() : isRunning(false), appName(""), priority(99) {}

// Destrutor
App::~App() {
}