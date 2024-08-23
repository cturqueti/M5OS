#ifndef SDMANAGER_H
#define SDMANAGER_H

#include <Arduino.h>

// #include "M5Cardputer.h"
#include "SD.h"
#include "SPI.h"
#include "Service.h"
#include "ServicesManager.h"
#include "Utils.h"
// #include <M5Cardputer.h>
#include <freertos/task.h>  // Inclui o cabeçalho adequado para o tipo TaskHandle_t

class SDManager : public Service {
   public:
    int scanRate;

    SDManager();
    virtual ~SDManager();
    static SDManager& getInstance() {
        static SDManager instance;
        return instance;
    }

    // Métodos virtuais puros que precisam ser implementados por classes derivadas
    void onServiceOpen()override;
    void onServiceTick()override;
    void onServiceClose()override;
    void draw()override;  // Função de desenho do serviço

    int servicePriority() const { return priority; }  // Pode ser sobrescrito

    inline bool isInitialized() { return SD.cardType() != CARD_NONE; }

    inline uint64_t totalSpace() { return isInitialized() ? (SD.totalBytes() / 1024 * 1024) : 0; }
    inline uint64_t usedSpace() { return isInitialized() ? (SD.usedBytes() / 1024 * 1024) : 0; }
    inline uint64_t freeSpace() { return isInitialized() ? (totalSpace() - usedSpace()) : 0; }

    // Acesso ao objeto SD
    decltype(SD)& getSD() {
        return SD;
    }

    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   protected:
    int lastMillis;
    uint64_t cardSize;
    SDCardPin sdCard;
    uint8_t priority;
    SPIClass SPI2;
    SDManager(SDManager const&) = delete;
    void operator=(SDManager const&) = delete;
};

#endif  // APP_H
