#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <algorithm>
#include <map>
#include <mutex>
#include <utility>
#include <vector>

#include "App.h"
#include "Utils.h"

#pragma once

class AppManager {
   public:
    // Singleton instance
    static AppManager& getInstance() {
        static AppManager instance;
        return instance;
    }

    // Métodos para gerenciar aplicativos
    void addApp(const std::string& appName, App* app);
    void openApp(const std::string& appName);
    void closeApp(const std::string& appName);
    App* getApp(const std::string& appName);
    void switchToApp(const std::string& appName);

    void startAppTask(const std::string& appName);

    // Lista aplicativos registrados
    std::vector<std::pair<std::string, App*>> listApps();

    // Função para imprimir informações de depuração
    void printDebugInfo();

    // Funções de execução
    bool removeTaskByName(const std::string& name);
    std::string getCurrentAppName() const;

    // Semáforos para controle de acesso
    SemaphoreHandle_t appSemaphore;
    SemaphoreHandle_t taskTableSemaphore;

   private:
    // Construtor e destrutor privados para o padrão Singleton
    AppManager();
    ~AppManager();

    TaskInfo* findTaskByName(const std::string& name);
    void countTasksPerCore(const std::vector<TaskInfo>& taskTable);
    void fixShowOrderGaps(std::vector<TaskInfo>& tasks);

    UBaseType_t core0Tasks = 0;
    UBaseType_t core1Tasks = 0;

    std::map<std::string, App*> apps;
    static std::vector<TaskInfo> taskTable;
    mutable std::mutex taskTableMutex;
    App* currentApp;

    // Prevenção de cópia
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};

#endif  // APP_MANAGER_H
