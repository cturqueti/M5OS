#include "AppManager.h"

#include <M5Cardputer.h>
#include <SD.h>

AppManager::AppManager() : currentAppName(""), currentApp(nullptr) {}

AppManager::~AppManager() {
    closeCurrentApp();
    for (auto& app : apps) {
        delete app.second;  // Libera memória dos aplicativos
    }
}

void AppManager::addApp(const String& appName, App* app) {
    if (apps.find(appName) == apps.end()) {
        apps[appName] = app;
    }
}

void AppManager::openApp(const String& appName) {
    closeCurrentApp();      // Fecha o app atual, se houver
    startAppTask(appName);  // Inicia a tarefa do novo app
    currentAppName = appName;
    currentApp = getApp(appName);  // Atualiza o app atual
}

void AppManager::closeApp(const String& appName) {
    if (apps.find(appName) != apps.end()) {
        App* app = apps[appName];
        vTaskDelete(app->getTaskHandle());  // Deleta a tarefa do app
        currentAppName = "";
        delete app;
        apps.erase(appName);  // Remove o app do mapa
    }
}

void AppManager::closeCurrentApp() {
    if (!currentAppName.isEmpty()) {
        closeApp(currentAppName);
    }
}

void AppManager::startAppTask(const String& appName) {
    App* app = getApp(appName);
    if (app) {
        // Captura o handle da tarefa no contexto correto
        xTaskCreatePinnedToCore(
            [](void* param) {
                App* app = static_cast<App*>(param);
                // Atualiza o handle da tarefa no contexto da tarefa criada
                app->setTaskHandle(xTaskGetCurrentTaskHandle());
                while (true) {
                    app->tick();                    // Chama a função de atualização do app
                    vTaskDelay(pdMS_TO_TICKS(15));  // Delay de 15ms
                }
            },
            appName.c_str(),
            8192,     // Tamanho da stack
            app,      // Parâmetro passado para a função
            1,        // Prioridade
            nullptr,  // Handle da tarefa
            1);       // Executar no Core 1
    }
}

App* AppManager::getApp(const String& appName) {
    auto it = apps.find(appName);
    if (it != apps.end()) {
        return it->second;
    }
    return nullptr;
}

String AppManager::getCurrentAppName() const {
    return currentAppName;
}

void AppManager::tickCurrentApp() {
    if (currentApp) {
        currentApp->tick();  // Atualiza o aplicativo atual
    }
}

void AppManager::draw() {
    if (currentApp) {
        currentApp->draw();  // Desenha o aplicativo atual
    }
}
