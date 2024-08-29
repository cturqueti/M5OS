#include "Globals.h"

// SemaphoreHandle_t appOpenSemaphore = nullptr;
// SemaphoreHandle_t serviceOpenSemaphore = nullptr;

// void initGlobals() {
//     appOpenSemaphore = xSemaphoreCreateMutex();
//     serviceOpenSemaphore = xSemaphoreCreateMutex();
//     if (appOpenSemaphore == nullptr) {
//         ESP_LOGE("Globals", "Falha ao criar o semáforo appOpenSemaphore");
//     }
//     if (serviceOpenSemaphore == nullptr) {
//         ESP_LOGE("Globals", "Falha ao criar o semáforo appOpenSemaphore");
//     }
// }

// bool takeAppOpenSemaphore(TickType_t ticksToWait) {
//     if (appOpenSemaphore != nullptr) {
//         if (xSemaphoreTake(appOpenSemaphore, ticksToWait) == pdTRUE) {
//             return true;
//         }
//     }
//     return false;
// }

// void giveAppOpenSemaphore() {
//     if (appOpenSemaphore != nullptr) {
//         xSemaphoreGive(appOpenSemaphore);
//     }
// }

// bool takeServiceOpenSemaphore(TickType_t ticksToWait) {
//     if (serviceOpenSemaphore != nullptr) {
//         if (xSemaphoreTake(serviceOpenSemaphore, ticksToWait) == pdTRUE) {
//             return true;
//         }
//     }
//     return false;
// }

// void giveServiceOpenSemaphore() {
//     if (serviceOpenSemaphore != nullptr) {
//         xSemaphoreGive(serviceOpenSemaphore);
//     }
// }