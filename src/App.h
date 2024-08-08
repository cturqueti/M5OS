#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <M5Cardputer.h>
#include <Task.h>  // Inclua o cabeçalho adequado para o tipo de TaskHandle_t

class App {
   public:
    App();
    virtual ~App();
    void begin();

    virtual void tick() = 0;  // Função de atualização do aplicativo
    virtual void draw() = 0;  // Função de desenho do aplicativo

    // Getters e Setters
    TaskHandle_t getTaskHandle() const;
    void setTaskHandle(TaskHandle_t handle);

    virtual size_t getIconSize() const { return 0; }            // Método padrão que deve ser sobrescrito
    virtual const uint8_t* getIcon() const { return nullptr; }  // Método padrão que deve ser sobrescrito

   protected:
    TaskHandle_t taskHandle;  // Membro protegido para o handle da tarefa
};

#endif  // APP_H
