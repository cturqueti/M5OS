#include "AppManager.h"

#include <SD.h>

#include "esp_log.h"

static const char* TAG = "AppManager";

std::vector<TaskInfo> AppManager::taskTable;

AppManager::AppManager() : currentApp(nullptr),
                           core0Tasks(0),
                           core1Tasks(0) {
    appSemaphore = xSemaphoreCreateMutex();
    if (appSemaphore == nullptr) {
        ESP_LOGE(TAG, "Falha ao criar o semáforo serviceSemaphore");
    }
    taskTableSemaphore = xSemaphoreCreateMutex();
    if (taskTableSemaphore == nullptr) {
        ESP_LOGE(TAG, "Falha ao criar o semáforo taskTableSemaphore");
    }
}

AppManager::~AppManager() {
    for (auto& app : apps) {
        delete app.second;  // Libera memória dos aplicativos
    }
    if (appSemaphore != nullptr) {
        vSemaphoreDelete(appSemaphore);
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
            ESP_LOGW(TAG, "Aplicativo %s já está em execução", appName.c_str());
            return;
        }
        // Atualiza o app atual
        currentApp = getApp(appName);

        if (currentApp) {
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
        TaskHandle_t taskHandle = app->getTaskHandle();
        ESP_LOGI(TAG, "Fechando a task para o aplicativo %s, ponteiro %p", appName.c_str(), taskHandle);

        vTaskDelay(pdMS_TO_TICKS(10));
        if (taskHandle) {
            app->onAppClose();
            // app->setClose(true);
            app->setTaskHandle(nullptr);  // Limpa o handle após excluir

            if (xSemaphoreTake(taskTableSemaphore, portMAX_DELAY) == pdTRUE) {
                removeTaskByName(appName);
                xSemaphoreGive(taskTableSemaphore);
            }

            // Tenta excluir a tarefa
            vTaskDelete(taskHandle);
            ESP_LOGI(TAG, "Task %s excluída com sucesso", appName.c_str());

        } else {
            ESP_LOGI(TAG, "Task handle para %s é nulo", appName.c_str());
        }

    } else {
        ESP_LOGW(TAG, "Aplicativo %s não encontrado", appName.c_str());
    }
}

App* AppManager::getApp(const std::string& appName) {
    auto it = apps.find(appName);
    if (it != apps.end()) {
        return it->second;
    }
    return nullptr;
}

void AppManager::switchToApp(const std::string& appName) {
    uint32_t nextShowOrder = 0;
    bool appFound = false;
    TaskInfo* task = findTaskByName(appName);

    if (!taskTable.empty()) {
        nextShowOrder = taskTable.back().showOrder + 1;
    }

    if (xSemaphoreTake(taskTableSemaphore, portMAX_DELAY) == pdTRUE) {
        if (task != nullptr) {
            // Atualiza a ordem de exibição da tarefa existente
            task->showOrder = nextShowOrder;
        } else {
            // Adiciona uma nova tarefa se não encontrada
            TaskInfo newTask{appName, nextShowOrder};
            taskTable.push_back(newTask);
        }
        fixShowOrderGaps(taskTable);
        xSemaphoreGive(taskTableSemaphore);
    }
}

void taskAppFunction(void* pvParameters) {
    // App* app = static_cast<App*>(pvParameters);
    auto params = static_cast<std::pair<AppManager*, App*>*>(pvParameters);

    if (!params) {
        ESP_LOGE(TAG, "Parâmetros nulos recebidos");
        vTaskDelete(nullptr);  // Exclui a própria tarefa se os parâmetros forem nulos
        return;
    }

    AppManager* appManager = params->first;
    App* app = params->second;

    app->setTaskHandle(xTaskGetCurrentTaskHandle());

    ESP_LOGV(TAG, "Iniciando loop da task para: %s, ponteiro: %p", app->getAppName().c_str(), app->getTaskHandle());
    app->setStarted(true);

    if (xSemaphoreTake(appManager->appSemaphore, portMAX_DELAY) == pdTRUE) {
        app->onAppOpen();
        app->setOpened(true);
        xSemaphoreGive(appManager->appSemaphore);
    }
    ESP_LOGV(TAG, "Iniciando loop da task para: %s", app->getAppName().c_str());

    vTaskDelay(pdMS_TO_TICKS(10));

    while (!app->isClosed()) {
        if (!app->isPaused()) {
            app->onAppTick();
            // app->draw();
            if (xSemaphoreTake(appManager->taskTableSemaphore, portMAX_DELAY) == pdTRUE) {
                if (app->getAppName() == appManager->getCurrentAppName()) {
                    app->draw();
                    app->setOnTop(true);
                } else {
                    app->setOnTop(false);
                }
                xSemaphoreGive(appManager->taskTableSemaphore);
            }
        }  // Chama a função de atualização do service
        vTaskDelay(pdMS_TO_TICKS(50));  // Delay de 1s
    }

    app->onAppClose();
    app->setTaskHandle(nullptr);  // Limpa o handle após excluir

    if (xSemaphoreTake(appManager->taskTableSemaphore, portMAX_DELAY) == pdTRUE) {
        appManager->removeTaskByName(app->getAppName());
        xSemaphoreGive(appManager->taskTableSemaphore);
    }
    delete params;
    vTaskDelete(nullptr);
}

void AppManager::startAppTask(const std::string& appName) {
    countTasksPerCore(taskTable);
    fixShowOrderGaps(taskTable);
    uint32_t nextShowOrder = 0;
    if (!taskTable.empty()) {
        nextShowOrder = taskTable.back().showOrder + 1;
    }

    // Seleciona o núcleo com menor carga
    int coreId = (core0Tasks > core1Tasks) ? 1 : 0;

    App* app = getApp(appName);
    if (app) {
        auto* params = new std::pair<AppManager*, App*>(this, app);
        TaskHandle_t taskHandle = nullptr;
        xTaskCreatePinnedToCore(
            taskAppFunction,
            appName.c_str(),
            8192,                // Tamanho da stack
            params,              // Parâmetro passado para a função
            app->appPriority(),  // Prioridade
            &taskHandle,         // Handle da tarefa
            coreId);             // Executar no Core 1

        // Armazena o handle da tarefa
        if (taskHandle) {
            app->setAppName(appName);

            ESP_LOGI(TAG, "Task %s criada com sucesso, ponteiro: %p", appName.c_str(), taskHandle);

            // Adiciona as informações à tabela
            TaskInfo taskInfo;
            taskInfo.appName = appName;
            taskInfo.priority = app->appPriority();  // A prioridade pode ser ajustada conforme necessário
            taskInfo.coreId = coreId;
            taskInfo.showOrder = nextShowOrder;
            taskTable.push_back(taskInfo);

        } else {
            ESP_LOGE(TAG, "Falha ao criar a task para %s", appName.c_str());
        }
    }
}

std::vector<std::pair<std::string, App*>> AppManager::listApps() {
    std::vector<std::pair<std::string, App*>> appsList;
    for (const auto& app : apps) {
        // appsList.push_back(std::make_pair(app.first.c_str(), app.second));
        appsList.emplace_back(app.first, app.second);
        // ESP_LOGI(TAG, "ProgramaAberto %s", app.first.c_str());
    }
    return appsList;
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

std::string AppManager::getCurrentAppName() const {
    if (!taskTable.empty()) {
        const TaskInfo& lastTask = taskTable.back();
        return lastTask.appName;
    } else {
        ESP_LOGW(TAG, "Nenhuma tarefa encontrada no taskTable");
        return "";  // Ou algum valor de retorno padrão ou código de erro
    }
}

// --- private --- //
TaskInfo* AppManager::findTaskByName(const std::string& appName) {
    bool appFound;
    for (auto& task : taskTable) {
        if (task.appName == appName) {
            return &task;
        }
    }
    return nullptr;
}

void AppManager::countTasksPerCore(const std::vector<TaskInfo>& taskTable) {
    core0Tasks = 0;
    core1Tasks = 0;
    // Itera sobre a tabela de tarefas e conta as tarefas em cada núcleo
    for (const auto& task : taskTable) {
        if (task.coreId == 0) {
            core0Tasks++;
        } else if (task.coreId == 1) {
            core1Tasks++;
        }
    }
}

void AppManager::fixShowOrderGaps(std::vector<TaskInfo>& tasks) {
    // Primeiro, ordenar por showOrder para garantir a sequência correta
    std::sort(tasks.begin(), tasks.end(), [](const TaskInfo& a, const TaskInfo& b) {
        return a.showOrder < b.showOrder;
    });

    // Percorrer o vetor e ajustar showOrder para corrigir os furos
    for (size_t i = 1; i < tasks.size(); ++i) {
        if (tasks[i].showOrder != tasks[i - 1].showOrder + 1) {
            // Ajuste o showOrder para preencher o furo
            tasks[i].showOrder = tasks[i - 1].showOrder + 1;
        }
    }
}
