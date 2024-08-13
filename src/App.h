#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <M5Cardputer.h>
#include <freertos/task.h>  // Inclui o cabeçalho adequado para o tipo TaskHandle_t

class App {
   public:
    App();
    virtual ~App();
    void begin();

    virtual void onAppOpen() = 0;
    virtual void onAppClose() = 0;
    virtual void onAppTick() = 0;  // Função de atualização do aplicativo
    virtual void draw() = 0;       // Função de desenho do aplicativo

    // Getters e Setters
    TaskHandle_t getTaskHandle() const;
    void setTaskHandle(TaskHandle_t handle);

    virtual const uint8_t* getIcon() {
        return nullptr;
    };
    virtual size_t getIconSize() {
        return 0;
    };

   private:
    TaskHandle_t taskHandle;  // Membro protegido para o handle da tarefa
};

#endif  // APP_H
