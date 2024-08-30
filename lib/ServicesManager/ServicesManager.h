#ifndef SERVICES_MANAGER_H
#define SERVICES_MANAGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "Service.h"
#include "Utils.h"

#pragma once

// extern SemaphoreHandle_t onServiceOpenSemaphore;

class ServicesManager {
   public:
    static ServicesManager& getInstance() {
        static ServicesManager instance;
        return instance;
    }

    void addService(const std::string& serviceName, Service* service);
    void openService(const std::string& name);
    void closeService(const std::string& name);
    void closeCurrentService();

    void startServiceTask(const std::string& appName);
    Service* getService(const std::string& serviceName);
    std::string getCurrentServiceName() const;
    void switchToService(const std::string& appName);
    std::vector<std::pair<std::string, Service*>> listServices();

    void printDebugInfo();

    SemaphoreHandle_t serviceSemaphore;

    bool removeTaskByName(const std::string& name);

   private:
    ServicesManager();
    ~ServicesManager();

    TaskInfo* findTaskByName(const std::string& name);

    void countTasksPerCore(const std::vector<TaskInfo>& taskTable);

    UBaseType_t core0Tasks = 0;
    UBaseType_t core1Tasks = 0;

    std::map<std::string, Service*> services;  // Mapeia nomes de serviços para instâncias
    static std::vector<TaskInfo> taskTable;

    std::string currentServiceName;
    Service* currentService;

    // Não permite a cópia
    ServicesManager(const ServicesManager&) = delete;
    ServicesManager& operator=(const ServicesManager&) = delete;
};

#endif