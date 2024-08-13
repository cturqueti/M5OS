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
    if (apps.find(appName) != apps.end()) {
        if (appTasks.find(appName) != appTasks.end()) {
            printf("Aplicativo %s já está em execução\n", appName.c_str());
            return;
        }
        printf("Adicionando App %s à task\n", appName.c_str());
        startAppTask(appName);
        currentAppName = appName;
        currentApp = getApp(appName);
        if (currentApp) {
            currentApp->onAppOpen();
        } else {
            printf("Erro ao abrir o aplicativo %s: referência nula\n", appName.c_str());
        }
    } else {
        printf("Aplicativo %s não encontrado\n", appName.c_str());
    }
}

void AppManager::closeApp(const String& appName) {
    printf("Tentando fechar o aplicativo %s\n", appName.c_str());

    // Verifica se a tarefa está em execução
    auto it = appTasks.find(appName);
    if (it != appTasks.end()) {
        // Verifica o identificador da tarefa
        TaskHandle_t taskHandle = it->second;
        printf("Task handle para %s é %p\n", appName.c_str(), taskHandle);  // Imprime o valor do handle

        if (taskHandle) {
            // Tenta excluir a tarefa
            vTaskDelete(taskHandle);
            delay(1000);
            printf("Task %s excluída com sucesso\n", appName.c_str());
        } else {
            printf("Task handle para %s é nulo\n", appName.c_str());
        }

        // Remove a tarefa do mapa
        appTasks.erase(it);
    } else {
        printf("Task para o aplicativo %s não encontrada\n", appName.c_str());
    }

    // Verifica se a tarefa foi excluída
    printf("Passou por aqui\n");

    // Verifica o mapa de aplicativos
    if (apps.find(appName) != apps.end()) {
        printf("Removendo o aplicativo %s do mapa\n", appName.c_str());
        App* app = apps[appName];
        delete app;
        apps.erase(appName);  // Remove o app do mapa
        printf("Aplicativo %s removido com sucesso\n", appName.c_str());
    } else {
        printf("Aplicativo %s não encontrado no mapa\n", appName.c_str());
    }
}

void AppManager::closeCurrentApp() {
    if (!currentAppName.isEmpty()) {
        if (currentApp) {
            currentApp->onAppClose();
        }
        String appName = currentAppName;  // Salva o nome antes de limpar
        currentAppName = "";
        currentApp = nullptr;
        closeApp(appName);  // Chama o closeApp

        // Atualiza o aplicativo no topo, se necessário
        if (!appTasks.empty()) {
            auto it = appTasks.begin();
            currentAppName = it->first;
            currentApp = getApp(currentAppName);
            if (currentApp) {
                currentApp->onAppOpen();
            }
        }
    }
}

void AppManager::startAppTask(const String& appName) {
    static bool useCore1 = true;  // Alterna entre os núcleos

    int coreId = useCore1 ? 1 : 0;
    useCore1 = !useCore1;

    App* app = getApp(appName);
    if (app) {
        TaskHandle_t taskHandle = nullptr;
        xTaskCreatePinnedToCore(
            [](void* param) {
                App* app = static_cast<App*>(param);
                app->setTaskHandle(xTaskGetCurrentTaskHandle());
                while (true) {
                    app->onAppTick();               // Chama a função de atualização do app
                    vTaskDelay(pdMS_TO_TICKS(15));  // Delay de 15ms
                }
            },
            appName.c_str(),
            8192,         // Tamanho da stack
            app,          // Parâmetro passado para a função
            1,            // Prioridade
            &taskHandle,  // Handle da tarefa
            coreId);      // Executar no Core 1

        // Armazena o handle da tarefa
        if (taskHandle) {
            printf("Task %s criada com sucesso, ponteiro: %p\n", appName.c_str(), taskHandle);
            appTasks[appName] = taskHandle;  // Armazena o handle apenas se a tarefa foi criada
        } else {
            printf("Falha ao criar a task para %s\n", appName.c_str());
        }
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
        currentApp->onAppTick();  // Atualiza o aplicativo atual
    }
}

void AppManager::draw() {
    if (currentApp) {
        currentApp->draw();  // Desenha o aplicativo atual
    }
}

std::vector<std::pair<std::string, App*>> AppManager::listApps() {
    std::vector<std::pair<std::string, App*>> appsList;
    for (const auto& app : apps) {
        appsList.push_back(std::make_pair(app.first.c_str(), app.second));
    }
    return appsList;
}

std::vector<String> AppManager::listOpenApps() const {
    std::vector<String> openApps;
    for (const auto& appTask : appTasks) {
        openApps.push_back(appTask.first);  // Adiciona o nome do aplicativo à lista
    }
    return openApps;
}

void AppManager::printDebugInfo() {
    printf("Aplicativos abertos:\n");
    for (const auto& appTask : appTasks) {
        printf("- %s\n", appTask.first.c_str());
    }
    printf("Aplicativo no topo: %s\n", currentAppName.c_str());
}