#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>

#include <map>

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
    void openBackgroundApp(const String& name);
    void closeApp(const String& appName);
    void closeCurrentApp();
    void startAppTask(const String& appName);
    App* getApp(const String& appName);
    String getCurrentAppName() const;
    void tickCurrentApp();
    void draw();
    std::vector<std::pair<std::string, App*>> listApps();

    void addBackgroundApp(const String& name);
    void removeBackgroundApp(const String& name);
    void drawBackgroundApps();

   private:
    AppManager();
    ~AppManager();

    std::map<String, App*> apps;  // Mapeia nomes de aplicativos para instâncias
    std::map<String, TaskHandle_t> appTasks;
    std::vector<String> backgroundApps;
    String currentAppName;
    App* currentApp;
    App* backgroundApp;

    void runBackgroundTasks();

    // Não permite a cópia
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;
};

#endif  // APP_MANAGER_H
