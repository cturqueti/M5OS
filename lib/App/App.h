#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "GlobalDisplay.h"

class App {
   public:
    App();
    virtual ~App();

    virtual void onAppOpen() = 0;
    virtual void onAppTick();  // Função de atualização do aplicativo
    virtual void onAppClose();
    virtual void draw() = 0;  // Função de desenho do aplicativo

    // Getters e Setters
    inline TaskHandle_t getTaskHandle() const { return taskHandle; };
    inline void setTaskHandle(TaskHandle_t handle) { taskHandle = handle; };
    // inline bool isTaskRunning() const { return isRunning; };

    inline std::string getAppName() const { return appName; }
    inline void setAppName(const std::string& name) { appName = name; }

    virtual int appPriority() const { return priority; }  // Pode ser sobrescrito
    virtual const uint8_t* getIcon() { return nullptr; };
    virtual size_t getIconSize() { return 0; };

    // inline bool isOpened() const { return isOpen; }      // Verifica se a aplicação está aberta
    // inline void setOpened(bool open) { isOpen = open; }  // Define o estado da aplicação
    // inline bool isClosed() const { return isClose; }
    // inline void setClosed(bool close) { isClose = close; }

    void setStarted(bool status) { _start = status; }
    void setOpened(bool status) { _open = status; }
    void setClosed(bool status) { _close = status; }
    void setPaused(bool status) { _pause = status; }
    void setOnTop(bool status) { _onTop = status; }
    bool isStarted() { return _start; }
    bool isOpened() { return _open; }
    bool isClosed() { return _close; }
    bool isPaused() { return _pause; }
    bool isOnTop() { return _onTop; }

   private:
    TaskHandle_t taskHandle;  // Membro protegido para o handle da tarefa
    std::string appName;

    uint8_t priority;
    // bool isRunning;

    // bool isOpen;
    // bool isClose;
    bool _start, _open, _close, _pause;
    bool _onTop;
};

#endif  // APP_H
