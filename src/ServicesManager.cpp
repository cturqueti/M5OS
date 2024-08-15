#include "ServicesManager.h"

#include <SD.h>

#include "esp_log.h"

static const char* TAG = "ServiceManager";

std::vector<TaskInfo> ServicesManager::taskTable;

ServicesManager::ServicesManager() : currentServiceName(""), currentService(nullptr), core0Tasks(0), core1Tasks(0) {}

ServicesManager::~ServicesManager() {
    removeCurrentService();
    for (auto& service : services) {
        delete service.second;  // Libera memória dos serviços
    }
}

void ServicesManager::addService(const std::string& serviceName, Service* service) {
    if (services.find(serviceName) == services.end()) {
        services[serviceName] = service;
        ESP_LOGD(TAG, "Service %s adicionado", serviceName.c_str());
    }
}

void ServicesManager::openService(const std::string& serviceName) {
    if (services.find(serviceName) != services.end()) {
        if (findTaskByName(serviceName)) {
            ESP_LOGW(TAG, "Serviço %s já está em execução", serviceName.c_str());
            return;
        }

        currentServiceName = serviceName;
        currentService = getService(serviceName);

        if (currentService) {
            ESP_LOGD(TAG, "Adicionando serviço %s à task", serviceName.c_str());
            startServiceTask(serviceName);
        } else {
            ESP_LOGE(TAG, "Erro ao abrir o serviço %s: referência nula", serviceName.c_str());
        }

    } else {
        ESP_LOGE(TAG, "Serviço %s não encontrado", serviceName.c_str());
    }
}

void ServicesManager::removeService(const std::string& serviceName) {
    ESP_LOGI(TAG, "Tentando fechar o serviço %s", serviceName.c_str());

    Service* service = getService(serviceName);
    if (service) {
        TaskHandle_t taskHandle = service->getTaskHandle();
        ESP_LOGI(TAG, "Fechando a task para o aplicativo %s", serviceName.c_str());

        vTaskDelay(pdMS_TO_TICKS(10));
        if (taskHandle) {
            service->onServiceClose();
            service->setTaskHandle(nullptr);  // Limpa o handle após excluir
            removeTaskByName(serviceName);
            // Tenta excluir a tarefa
            vTaskDelete(taskHandle);
            ESP_LOGI(TAG, "Task %s excluída com sucesso", serviceName.c_str());

        } else {
            ESP_LOGI(TAG, "Task handle para %s é nulo", serviceName.c_str());
        }

    } else {
        ESP_LOGW(TAG, "Serviço %s não encontrado", serviceName.c_str());
    }
}

void ServicesManager::removeCurrentService() {
    if (!currentServiceName.empty()) {
        if (currentService) {
            currentService->onServiceClose();
        }
        std::string serviceName = currentServiceName;
        currentServiceName = "";
        currentService = nullptr;
        removeService(serviceName);

        if (!taskTable.empty()) {
            auto it = taskTable.begin();
            currentServiceName = it->appName;
            currentService = getService(currentServiceName);
            if (currentService) {
                currentService->onServiceOpen();
            }
        }
    }
}

void ServicesManager::switchToService(const std::string& serviceName) {
    if (services.find(serviceName) != services.end()) {
        openService(serviceName);
    } else {
        ESP_LOGE(TAG, "Aplicativo %s não encontrado para troca", serviceName.c_str());
    }
}

void taskServicesFunction(void* pvParameters) {
    Service* service = static_cast<Service*>(pvParameters);
    service->setTaskHandle(xTaskGetCurrentTaskHandle());

    ESP_LOGV(TAG, "Iniciando loop da task para: %s", service->getServiceName().c_str());

    while (true) {
        service->onServiceTick();  // Chama a função de atualização do service
        // service->draw(); // not used
        // ESP_LOGV(TAG, "Tick: %s", service->getServiceName().c_str());
        vTaskDelay(pdMS_TO_TICKS(15));  // Delay de 15ms
    }
}

void ServicesManager::startServiceTask(const std::string& serviceName) {
    countTasksPerCore(taskTable);
    // Seleciona o núcleo com menor carga
    int coreId = (core0Tasks > core1Tasks) ? 1 : 0;

    Service* service = getService(serviceName);
    if (service) {
        TaskHandle_t taskHandle = nullptr;
        xTaskCreatePinnedToCore(
            taskServicesFunction,
            serviceName.c_str(),
            8192,                        // Tamanho da stack
            service,                     // Parâmetro passado para a função
            service->servicePriority(),  // Prioridade
            &taskHandle,                 // Handle da tarefa
            coreId);                     // Executar no Core 1

        // Armazena o handle da tarefa
        if (taskHandle) {
            service->setTaskHandle(taskHandle);
            service->setServiceName(serviceName);
            vTaskDelay(pdMS_TO_TICKS(10));
            ESP_LOGI(TAG, "Task %s criada com sucesso, ponteiro: %p", serviceName.c_str(), taskHandle);

            // Adiciona as informações à tabela
            TaskInfo taskInfo;
            taskInfo.appName = serviceName;
            taskInfo.priority = service->servicePriority();  // A prioridade pode ser ajustada conforme necessário
            taskInfo.coreId = coreId;
            taskTable.push_back(taskInfo);
            service->onServiceOpen();
        } else {
            ESP_LOGE(TAG, "Falha ao criar a task para %s", serviceName.c_str());
        }
    }
}

Service* ServicesManager::getService(const std::string& serviceName) {
    auto it = services.find(serviceName);
    if (it != services.end()) {
        return it->second;
    }
    return nullptr;
}

std::string ServicesManager::getCurrentServiceName() const {
    return currentServiceName;
}

std::vector<std::pair<std::string, Service*>> ServicesManager::listServices() {
    std::vector<std::pair<std::string, Service*>> servicesList;
    for (const auto& service : services) {
        servicesList.push_back(std::make_pair(service.first.c_str(), service.second));
    }
    return servicesList;
}

TaskInfo* ServicesManager::findTaskByName(const std::string& name) {
    auto it = std::find_if(taskTable.begin(), taskTable.end(), [&name](const TaskInfo& taskInfo) {
        return taskInfo.appName == name;
    });
    if (it != taskTable.end()) {
        return &(*it);  // Retorna um ponteiro para o TaskInfo encontrado
    }
    return nullptr;  // Retorna nullptr se não encontrar
}

bool ServicesManager::removeTaskByName(const std::string& name) {
    auto it = std::remove_if(taskTable.begin(), taskTable.end(), [&name](const TaskInfo& taskInfo) {
        return taskInfo.appName == name;
    });

    if (it != taskTable.end()) {
        taskTable.erase(it, taskTable.end());  // Remove os elementos encontrados
        ESP_LOGI("ServiceManager", "Tarefa %s removida com sucesso", name.c_str());
        return true;
    } else {
        ESP_LOGW("ServiceManager", "Tarefa %s não encontrada", name.c_str());
    }
    return false;
}

void ServicesManager::printDebugInfo() {
    ESP_LOGI(TAG, "Número de tarefas na tabela: %d", static_cast<int>(taskTable.size()));
    for (const auto& pair : services) {
        // A chave (key) do mapa é o nome do aplicativo, que é do tipo String
        ESP_LOGI("ServiceManager", "Service Name: %s", pair.first.c_str());
    }

    for (const auto& taskInfo : taskTable) {
        ESP_LOGI(TAG, "Aplicativo: %s, Prioridade: %u, Core ID: %d",
                 taskInfo.appName.c_str(),
                 taskInfo.priority,
                 taskInfo.coreId);
    }
}

void ServicesManager::countTasksPerCore(const std::vector<TaskInfo>& taskTable) {
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