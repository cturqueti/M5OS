#include "App.h"
  // Assumindo que o ScreenManager tem a definição do canvas
#include "esp_log.h"
static const char* TAG = "App";

// Construtor
App::App() : isRunning(false), appName(""), priority(99) {}

// Destrutor
App::~App() {
}