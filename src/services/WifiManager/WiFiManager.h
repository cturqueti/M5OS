#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>

// #include "M5Cardputer.h"
#include "SD.h"
#include "SPI.h"
#include "Service.h"
#include "ServicesManager.h"
#include "Utils.h"
// #include <M5Cardputer.h>
#include <freertos/task.h>  // Inclui o cabeçalho adequado para o tipo TaskHandle_t

#include "WifiConnected.h"
#include "WifiDisconnected.h"

class WiFiManager : public Service {
   public:
    int scanRate;

    WiFiManager();
    virtual ~WiFiManager();

    static WiFiManager& getInstance() {
        static WiFiManager instance;
        return instance;
    }

    // Métodos virtuais puros que precisam ser implementados por classes derivadas
    void onServiceOpen();
    void onServiceTick();
    void onServiceClose();
    void draw();  // Função de desenho do serviço

    int servicePriority() const { return priority; }  // Pode ser sobrescrito

    inline bool isConnected() { return connected; }

    const uint8_t* getIcon() override;  // os icones devem ser PNG tamanho 12X12
    size_t getIconSize() override;

   protected:
    uint8_t connected;
    int lastMillis;
    uint8_t priority;
    WiFiManager(WiFiManager const&) = delete;
    void operator=(WiFiManager const&) = delete;
};

#endif  // APP_H
