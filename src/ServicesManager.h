#ifndef SERVICES_MANAGER_H
#define SERVICES_MANAGER_H

#include <Arduino.h>

#include <map>
#include <string>
#include <vector>

#include "Service.h"

class ServicesManager {
   public:
    static ServicesManager& getInstance() {
        static ServicesManager instance;
        return instance;
    }

    void addService(const String& serviceName, Service* service);
    void openService(const String& name);
    void removeService(const String& name);
    void tickServices();
    void drawServices();

   private:
    ServicesManager() {}
    ~ServicesManager();

    std::map<String, Service*> services;  // Mapeia nomes de serviços para instâncias
    std::vector<String> runningServices;
    std::map<String, TaskHandle_t> serviceTasks;

    void startServiceTask(const String& serviceName);

    // Não permite a cópia
    ServicesManager(const ServicesManager&) = delete;
    ServicesManager& operator=(const ServicesManager&) = delete;
};

#endif