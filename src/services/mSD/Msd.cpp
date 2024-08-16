#include "Msd.h"

#include "esp_log.h"
static const char* TAG = "Services";

// Construtor
Msd::Msd() : priority(99) {}

// Destrutor
Msd::~Msd() {
}

void Msd::onServiceOpen() {
    // Inicializa o SPI com os pinos especificados
    SPI2.begin(sdCard.sck,   // CLK
               sdCard.miso,  // MISO
               sdCard.mosi,  // MOSI
               sdCard.cs);   // SS
                             //  Tenta iniciar o cartão SD utilizando o pino SS especificado
    uint16_t timeout = 600;
    while (!SD.begin(sdCard.cs, SPI2) || timeout < 1) {
        delay(10);
        timeout -= 1;
    }
    if (timeout > 0) {
        printf("MicroSd Ok\n");
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE) {
            printf("No SD card attached\n");

        } else {
            Serial.print("SD Card Type: ");
            if (cardType == CARD_MMC) {
                printf("MMC\n");
            } else if (cardType == CARD_SD) {
                printf("SDSC\n");
            } else if (cardType == CARD_SDHC) {
                printf("SDHC\n");
            } else {
                printf("UNKNOWN\n");
            }
            uint64_t cardSize = SD.cardSize() / (1024 * 1024);
            Serial.printf("SD Card Size: %lluMB\n", cardSize);
        }
    } else {
        printf("Error");
    }
}
void Msd::onServiceTick() {}
void Msd::onServiceClose() {}

void Msd::draw() {}