#ifndef SERVICE_H
#define SERVICE_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Service {
   public:
    Service();
    virtual ~Service();

    // Métodos virtuais puros que precisam ser implementados por classes derivadas
    virtual void onServiceOpen() = 0;
    virtual void onServiceTick() = 0;
    virtual void onServiceClose() = 0;
    virtual void draw() = 0;  // Função de desenho do serviço
    virtual const uint8_t* getIcon() = 0;
    virtual size_t getIconSize() = 0;

    inline TaskHandle_t getTaskHandle() const { return taskHandle; };
    inline void setTaskHandle(TaskHandle_t handle) { taskHandle = handle; };
    inline bool isTaskRunning() const { return isRunning; };

    inline std::string getServiceName() const { return serviceName; }
    inline void setServiceName(const std::string& name) { serviceName = name; }

    virtual int servicePriority() const { return priority; }  // Pode ser sobrescrito

      protected:
    uint8_t priority;
    bool isRunning;
    static TaskHandle_t taskHandle;
    std::string serviceName;
};

#endif  // APP_H
