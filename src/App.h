#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class App {
   public:
    App();
    virtual ~App();

    virtual void onAppOpen() = 0;
    virtual void onAppTick() = 0;  // Função de atualização do aplicativo
    virtual void onAppClose() = 0;
    virtual void draw() = 0;  // Função de desenho do aplicativo

    // Getters e Setters
    inline TaskHandle_t getTaskHandle() const { return taskHandle; };
    inline void setTaskHandle(TaskHandle_t handle) { taskHandle = handle; };
    inline bool isTaskRunning() const { return isRunning; };

    inline std::string getAppName() const { return appName; }
    inline void setAppName(const std::string& name) { appName = name; }

    virtual int appPriority() const { return priority; }  // Pode ser sobrescrito
    virtual const uint8_t* getIcon() { return nullptr; };
    virtual size_t getIconSize() { return 0; };

   private:
    TaskHandle_t taskHandle;  // Membro protegido para o handle da tarefa
    uint8_t priority;
    bool isRunning;

    std::string appName;
};

#endif  // APP_H
