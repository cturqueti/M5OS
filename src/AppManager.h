#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <map>
#include <utility>
#include <vector>

#include "App.h"

class AppManager {
   public:
    // Singleton instance
    static AppManager& getInstance() {
        static AppManager instance;
        return instance;
    }

    // Métodos para gerenciar aplicativos
    void addApp(const String& appName, App* app);
    void openApp(const String& appName);
    void closeApp(const String& appName);
    void closeCurrentApp();
    void startAppTask(const String& appName);
    App* getApp(const String& appName);
    String getCurrentAppName() const;
    void switchToApp(const String& appName);
    void tickCurrentApp();
    void draw();
    std::vector<std::pair<std::string, App*>> listApps();

    std::vector<String> listOpenApps() const;

    void printDebugInfo();

   private:
    AppManager();
    ~AppManager();

    std::map<String, App*> apps;  // Mapeia nomes de aplicativos para instâncias
    std::map<String, TaskHandle_t> appTasks;
    String currentAppName;
    App* currentApp;

    // Não permite a cópia
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};

#endif  // APP_MANAGER_H
