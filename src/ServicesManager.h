#ifndef SERVICES_MANAGER_H
#define SERVICES_MANAGER_H

#include <Arduino.h>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "Service.h"
#include "Utils.h"

#pragma once

class ServicesManager {
   public:
    static ServicesManager& getInstance() {
        static ServicesManager instance;
        return instance;
    }

    void addService(const std::string& serviceName, Service* service);
    void openService(const std::string& name);
    void removeService(const std::string& name);
    void removeCurrentService();

    void startServiceTask(const std::string& appName);
    Service* getService(const std::string& serviceName);
    std::string getCurrentServiceName() const;
    void switchToService(const std::string& appName);
    std::vector<std::pair<std::string, Service*>> listServices();

    void printDebugInfo();

   private:
    ServicesManager();
    ~ServicesManager();

    TaskInfo* findTaskByName(const std::string& name);
    bool removeTaskByName(const std::string& name);
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