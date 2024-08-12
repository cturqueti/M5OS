#include "ServicesManager.h"

#include <M5Cardputer.h>
#include <SD.h>

ServicesManager::~ServicesManager() {
    for (auto& service : serviceTasks) {
        vTaskDelete(service.second);  // Libera a task
    }
    for (auto& service : services) {
        delete service.second;  // Libera memória dos serviços
    }
}

void ServicesManager::addService(const String& serviceName, Service* service) {
    if (services.find(serviceName) == services.end()) {
        services[serviceName] = service;
    }
}

void ServicesManager::openService(const String& serviceName) {
    if (services.find(serviceName) != services.end()) {
        if (serviceTasks.find(serviceName) != serviceTasks.end()) {
            printf("Aplicativo %s já está em execução\n", serviceName.c_str());
            return;
        }
        printf("Adicionando Service %s à task\n", serviceName.c_str());
        startServiceTask(serviceName);
        runningServices.push_back(serviceName);
        Service* service = services[serviceName];
        if (service) {
            service->onServiceOpen();
        } else {
            printf("Erro ao abrir o aplicativo %s: referência nula\n", serviceName.c_str());
        }
    } else {
        printf("Aplicativo %s não encontrado\n", serviceName.c_str());
    }
}

void ServicesManager::removeService(const String& serviceName) {
    auto it = serviceTasks.find(serviceName);
    if (it != serviceTasks.end()) {
        vTaskDelete(it->second);
        serviceTasks.erase(it);
    }

    auto itRunning = std::find(runningServices.begin(), runningServices.end(), serviceName);
    if (itRunning != runningServices.end()) {
        runningServices.erase(itRunning);
    }

    if (services.find(serviceName) != services.end()) {
        Service* service = services[serviceName];
        delete service;
        services.erase(serviceName);  // Remove o serviço do mapa
    }
}

void ServicesManager::tickServices() {
    for (const auto& serviceName : runningServices) {
        Service* service = services[serviceName];
        if (service) {
            service->onServiceTick();  // Atualize o serviço de segundo plano
        }
    }
}

void ServicesManager::drawServices() {
    for (const auto& serviceName : runningServices) {
        Service* service = services[serviceName];
        if (service) {
            service->draw();  // Desenha o serviço em segundo plano
        }
    }
}

void ServicesManager::startServiceTask(const String& serviceName) {
    if (serviceName.isEmpty() || services.find(serviceName) == services.end()) {
        printf("Nome do serviço inválido ou serviço não encontrado\n");
        return;
    }
    static bool useCore1 = true;
    int coreId = useCore1 ? 1 : 0;
    useCore1 = !useCore1;

    Service* service = services[serviceName];
    if (service) {
        TaskHandle_t taskHandle = nullptr;
        xTaskCreatePinnedToCore(
            [](void* param) {
                Service* service = static_cast<Service*>(param);
                service->setTaskHandle(xTaskGetCurrentTaskHandle());
                while (true) {
                    service->onServiceTick();
                    vTaskDelay(pdMS_TO_TICKS(15));
                }
            },
            serviceName.c_str(),
            8192,
            service,
            1,
            &taskHandle,
            coreId);

        if (taskHandle) {
            serviceTasks[serviceName] = taskHandle;
        } else {
            printf("Falha ao criar a task para %s\n", serviceName.c_str());
        }
    }
}
