#ifndef MSD_H
#define MSD_H

#include <Arduino.h>

// #include "M5Cardputer.h"
#include "SD.h"
#include "SPI.h"
#include "Service.h"
#include "ServicesManager.h"
#include "Utils.h"
// #include <M5Cardputer.h>
#include <freertos/task.h>  // Inclui o cabeçalho adequado para o tipo TaskHandle_t

class Msd : public Service {
   public:
    Msd();
    virtual ~Msd();

    // Métodos virtuais puros que precisam ser implementados por classes derivadas
    void onServiceOpen();
    void onServiceTick();
    void onServiceClose();
    void draw();  // Função de desenho do serviço

    int servicePriority() const { return priority; }  // Pode ser sobrescrito

   protected:
    SDCardPin sdCard;
    uint8_t priority;
    SPIClass SPI2;
};

#endif  // APP_H
