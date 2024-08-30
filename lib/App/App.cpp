#include "App.h"
// Assumindo que o ScreenManager tem a definição do canvas
#include "esp_log.h"
static const char* TAG = "App";

// Construtor
App::App() : appName(""),
             priority(99),
             _start(false),
             _open(false),
             _close(false),
             _pause(false) {
}

// Destrutor
App::~App() {
}

void App::onAppTick() {
    if (!isOpened()) {
        ESP_LOGI(TAG, "Aplicativo %s não está aberto, saindo de onAppTick", appName.c_str());

        return;  // Se a aplicação não estiver aberta, sair da função
    }
    // Lógica de atualização do aplicativo
}

void App::onAppClose() {
    if (isClosed()) {
        ESP_LOGI(TAG, "Aplicativo %s já está fechado", appName.c_str());
        return;
    }
    // Lógica para quando o aplicativo for fechado
    _close = true;  // Marcar o aplicativo como fechado
    ESP_LOGI(TAG, "Aplicativo %s foi fechado", appName.c_str());
}