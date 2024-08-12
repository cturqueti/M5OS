#ifndef SERVICE_H
#define SERVICE_H

#include <Arduino.h>
#include <M5Cardputer.h>
#include <freertos/task.h>  // Inclui o cabeçalho adequado para o tipo TaskHandle_t

class Service {
   public:
    Service();
    virtual ~Service();
    void begin();

    // Métodos virtuais puros que precisam ser implementados por classes derivadas
    virtual void onServiceOpen() = 0;
    virtual void onServiceClose() = 0;
    virtual void onServiceTick() = 0;
    virtual void draw() = 0;  // Função de desenho do serviço

    // Getters e Setters
    TaskHandle_t getTaskHandle() const;
    void setTaskHandle(TaskHandle_t handle);

    virtual const uint8_t* getIcon() {
        return nullptr;
    };
    virtual size_t getIconSize() {
        return 0;
    };

   protected:
    TaskHandle_t taskHandle;  // Membro protegido para o handle da tarefa
};

#endif  // APP_H
