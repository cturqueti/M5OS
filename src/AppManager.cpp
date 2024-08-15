#include "AppManager.h"

#include <SD.h>

#include "esp_log.h"

static const char* TAG = "AppManager";

AppManager::AppManager() : currentAppName(""), currentApp(nullptr) {}

AppManager::~AppManager() {
    closeCurrentApp();
    for (auto& app : apps) {
        delete app.second;  // Libera memória dos aplicativos
    }
}

void AppManager::addApp(const std::string& appName, App* app) {
    if (apps.find(appName) == apps.end()) {
        apps[appName] = app;
        ESP_LOGD(TAG, "Aplicativo %s adicionado", appName.c_str());
    }
}

void AppManager::openApp(const std::string& appName) {
    if (apps.find(appName) != apps.end()) {
        if (findTaskByName(appName)) {
            // if (appTasks.find(appName) != appTasks.end()) {
            ESP_LOGW(TAG, "Aplicativo %s já está em execução", appName.c_str());
            return;
        }
        // Atualiza o app atual
        currentAppName = appName;
        currentApp = getApp(appName);

        if (currentApp) {
            // Primeiro, chama onAppOpen
            currentApp->onAppOpen();

            // Depois, inicia a task do aplicativo
            ESP_LOGD(TAG, "Adicionando App %s à task", appName.c_str());
            startAppTask(appName);
        } else {
            ESP_LOGE(TAG, "Erro ao abrir o aplicativo %s: referência nula", appName.c_str());
        }
    } else {
        ESP_LOGE(TAG, "Aplicativo %s não encontrado", appName.c_str());
    }
}

void AppManager::closeApp(const std::string& appName) {
    ESP_LOGI(TAG, "Tentando fechar o aplicativo %s", appName.c_str());

    App* app = getApp(appName);
    if (app) {
        if (findTaskByName(appName)) {
            ESP_LOGI(TAG, "Removendo o aplicativo %s do mapa", appName.c_str());
            if (removeTaskByName(appName)) {
                ESP_LOGI(TAG, "Aplicativo %s removido com sucesso", appName.c_str());
            } else {
                ESP_LOGI(TAG, "Aplicativo %s não encontrado no mapa", appName.c_str());
            }
        }

        printDebugInfo();

        TaskHandle_t taskHandle = app->getTaskHandle();
        ESP_LOGI(TAG, "Fechando a task para o aplicativo %s", appName.c_str());
        if (app) {
            app->onAppClose();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
        if (taskHandle) {
            // Tenta excluir a tarefa
            vTaskDelete(taskHandle);
            ESP_LOGI(TAG, "Task %s excluída com sucesso", appName.c_str());
            app->setTaskHandle(nullptr);  // Limpa o handle após excluir

            removeTaskByName(appName);
        } else {
            ESP_LOGI(TAG, "Task handle para %s é nulo", appName.c_str());
        }

    } else {
        ESP_LOGW(TAG, "Aplicativo %s não encontrado", appName.c_str());
    }
}

void AppManager::closeCurrentApp() {
    if (!currentAppName.empty()) {
        if (currentApp) {
            currentApp->onAppClose();
        }
        std::string appName = currentAppName;
        currentAppName = "";
        currentApp = nullptr;
        closeApp(appName);

        if (!taskTable.empty()) {
            auto it = taskTable.begin();
            currentAppName = it->appName;
            currentApp = getApp(currentAppName);
            if (currentApp) {
                currentApp->onAppOpen();
            }
        }
    }
}

void AppManager::switchToApp(const std::string& appName) {
    if (apps.find(appName) != apps.end()) {
        openApp(appName);
    } else {
        ESP_LOGE(TAG, "Aplicativo %s não encontrado para troca", appName.c_str());
    }
}

void taskFunction(void* pvParameters) {
    App* app = static_cast<App*>(pvParameters);
    app->setTaskHandle(xTaskGetCurrentTaskHandle());

    ESP_LOGV(TAG, "Iniciando loop da task para: %s", app->getAppName().c_str());

    while (true) {
        app->onAppTick();  // Chama a função de atualização do app
        // ESP_LOGV(TAG, "Tick: %s", app->getAppName().c_str());
        vTaskDelay(pdMS_TO_TICKS(15));  // Delay de 15ms
    }
}

void AppManager::startAppTask(const std::string& appName) {
    static bool useCore1 = true;  // Alterna entre os núcleos

    int coreId = useCore1 ? 1 : 0;
    useCore1 = !useCore1;

    App* app = getApp(appName);
    if (app) {
        TaskHandle_t taskHandle = nullptr;
        xTaskCreatePinnedToCore(
            taskFunction,
            appName.c_str(),
            8192,                // Tamanho da stack
            app,                 // Parâmetro passado para a função
            app->appPriority(),  // Prioridade
            &taskHandle,         // Handle da tarefa
            coreId);             // Executar no Core 1

        // Armazena o handle da tarefa
        if (taskHandle) {
            app->setTaskHandle(taskHandle);
            app->setAppName(appName);
            vTaskDelay(pdMS_TO_TICKS(10));
            ESP_LOGI(TAG, "Task %s criada com sucesso, ponteiro: %p", appName.c_str(), taskHandle);

            // Adiciona as informações à tabela
            TaskInfo taskInfo;
            taskInfo.appName = appName;
            taskInfo.priority = app->appPriority();  // A prioridade pode ser ajustada conforme necessário
            taskInfo.coreId = coreId;
            taskTable.push_back(taskInfo);
        } else {
            ESP_LOGE(TAG, "Falha ao criar a task para %s", appName.c_str());
        }
    }
}

App* AppManager::getApp(const std::string& appName) {
    auto it = apps.find(appName);
    if (it != apps.end()) {
        return it->second;
    }
    return nullptr;
}

std::string AppManager::getCurrentAppName() const {
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

TaskInfo* AppManager::findTaskByName(const std::string& name) {
    auto it = std::find_if(taskTable.begin(), taskTable.end(), [&name](const TaskInfo& taskInfo) {
        return taskInfo.appName == name;
    });
    if (it != taskTable.end()) {
        return &(*it);  // Retorna um ponteiro para o TaskInfo encontrado
    }
    return nullptr;  // Retorna nullptr se não encontrar
}

bool AppManager::removeTaskByName(const std::string& name) {
    auto it = std::remove_if(taskTable.begin(), taskTable.end(), [&name](const TaskInfo& taskInfo) {
        return taskInfo.appName == name;
    });

    if (it != taskTable.end()) {
        taskTable.erase(it, taskTable.end());  // Remove os elementos encontrados
        ESP_LOGI("AppManager", "Tarefa %s removida com sucesso", name.c_str());
        return true;
    } else {
        ESP_LOGW("AppManager", "Tarefa %s não encontrada", name.c_str());
    }
    return false;
}

void AppManager::printDebugInfo() {
    ESP_LOGI(TAG, "Número de tarefas na tabela: %d", static_cast<int>(taskTable.size()));
    for (const auto& pair : apps) {
        // A chave (key) do mapa é o nome do aplicativo, que é do tipo String
        ESP_LOGI("AppManager", "App Name: %s", pair.first.c_str());
    }

    for (const auto& taskInfo : taskTable) {
        ESP_LOGI(TAG, "Aplicativo: %s, Prioridade: %u, Core ID: %d",
                 taskInfo.appName.c_str(),
                 taskInfo.priority,
                 taskInfo.coreId);
    }
}