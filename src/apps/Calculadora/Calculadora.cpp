#include "Calculadora.h"

#include "ScreenManager.h"

Calculadora::Calculadora() : tempCanvas(&M5.Lcd) {
    tempCanvas.createSprite(240, 135);
}

Calculadora::~Calculadora() {
    tempCanvas.deleteSprite();
}

void Calculadora::onAppTick() {
    // printf("Testando o programa");
    // delay(1000);
    //  Lógica de atualização do aplicativo
    if (Keyboard::getInstance().commandIsChange() && AppManager::getInstance().getCurrentAppName() == "Calculadora") {
        String command = Keyboard::getInstance().getCommand();
        printf("Comando recebido pela calculadora: ");
        if (command == "EXIT") {
            printf("Fechando app atual\n");
            AppManager::getInstance().closeCurrentApp();
        }
        if (command == "ENTER") {
            printf("Comando Enter\n");
        }
    }
}

void Calculadora::onAppOpen() {
    printf("Iniciando Calculadora...\n");
    // printf("Tamanho do ícone: %zu\n", getIconSize());

    const uint8_t* icon = getIcon();
    if (icon) {
        printf("Ícone carregado com sucesso.\n");
    } else {
        printf("Erro ao carregar ícone.\n");
    }

    StatusBar::draw();
    DrawingArea area = StatusBar::area;
    tempCanvas.createSprite(area.width, area.height);
    tempCanvas.fillSprite(TFT_BLACK);
}

void Calculadora::onAppClose() {
    // AppManager::getInstance().openApp("Launcher");
    //  apps.clear();
}

void Calculadora::draw() {
    // Limpa o canvas temporário
    DrawingArea area = StatusBar::area;

    tempCanvas.drawPng(getIcon(), getIconSize(), 50, 50, 75, 75);
    tempCanvas.setTextSize(1);
    tempCanvas.setTextColor(TFT_GREEN);
    tempCanvas.setCursor(5, 5);
    tempCanvas.print("Calculadora");

    // Sincroniza a operação com o canvas definitivo usando o semáforo global
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        M5Canvas& canvas = ScreenManager::getCanvas();

        // Copia o conteúdo do canvas temporário para o canvas definitivo
        tempCanvas.pushSprite(&canvas, area.xStart, area.yStart);

        // Libera o semáforo após o uso
        xSemaphoreGive(canvasSemaphore);
    }
}

const uint8_t* Calculadora::getIcon() {
    return calc_icon;
}

size_t Calculadora::getIconSize() {
    return sizeof(calc_icon);
}