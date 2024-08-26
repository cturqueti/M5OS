#include "Launcher.h"

#include "esp_log.h"

static const char* TAG = "Launcher";

Launcher::Launcher() {
}

Launcher::~Launcher() {
}

void Launcher::onAppOpen() {
    ESP_LOGI(TAG, "Iniciando delay de 800");
    printf("Iniciando Launcher.\n");
    nloop = 800;
}

void Launcher::onAppTick() {
    // nloop -= 1;
    // if (nloop % 10 == 0) {
    //     ESP_LOGI(TAG, "Rodando");
    // }
    // // ESP_LOGI(TAG, "Fazendo algo no Launcher");
    // if (nloop < 1) {
    //     AppManager::getInstance().closeApp("Launcher");
    // }
}

void Launcher::onAppClose() {
    ESP_LOGI(TAG, "Fechando Launcher");
    nloop = 800;
    ESP_LOGI(TAG, "Abrindo Calculadora");
    AppManager::getInstance().openApp("Calculadora");
}

void Launcher::draw() {
    GlobalDisplay& displayManager = GlobalDisplay::getInstance();
    SemaphoreHandle_t sem = displayManager.getSemaphore();

    if (xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE) {
        M5Canvas* canvas = displayManager.getCanvas();

        // Código de desenho no canvas
        // ...
        // canvas->pushSprite(0, 0);
        xSemaphoreGive(sem);  // Libere o semáforo após o uso
    }
}
