#include "App.h"
// Assumindo que o ScreenManager tem a definição do canvas
#include "esp_log.h"
static const char* TAG = "App";

// Construtor
App::App() : isOpen(false), isClose(false), taskHandle(nullptr), isRunning(false), appName(""), priority(99) {
    onAppOpenSemaphore = xSemaphoreCreateMutex();
    if (onAppOpenSemaphore == nullptr) {
        ESP_LOGE(TAG, "Falha ao criar o semáforo onAppOpenSemaphore");
    }
}

// Destrutor
App::~App() {
    if (onAppOpenSemaphore != nullptr) {
        vSemaphoreDelete(onAppOpenSemaphore);
    }
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
    isClose = true;  // Marcar o aplicativo como fechado
    ESP_LOGI(TAG, "Aplicativo %s foi fechado", appName.c_str());
}