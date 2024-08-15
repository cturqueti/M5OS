#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "App.h"

struct TaskInfo {
    std::string appName;
    UBaseType_t priority;
    uint8_t coreId;
};

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
    void closeCurrentApp();
    void startAppTask(const std::string& appName);
    App* getApp(const std::string& appName);
    std::string getCurrentAppName() const;
    void switchToApp(const std::string& appName);
    void tickCurrentApp();
    void draw();
    std::vector<std::pair<std::string, App*>> listApps();

    void printDebugInfo();

   private:
    AppManager();
    ~AppManager();

    TaskInfo* findTaskByName(const std::string& name);
    bool removeTaskByName(const std::string& name);
    void countTasksPerCore(const std::vector<TaskInfo>& taskTable);

    UBaseType_t core0Tasks = 0;
    UBaseType_t core1Tasks = 0;

    std::map<std::string, App*> apps;  // Mapeia nomes de aplicativos para instâncias
    std::vector<TaskInfo> taskTable;
    std::string currentAppName;
    App* currentApp;

    // Não permite a cópia
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};

#endif  // APP_MANAGER_H
