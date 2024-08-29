#include "Calculadora.h"

static const char* TAG = "Calculadora";

Calculadora::Calculadora() : centerSizes(&GlobalDisplay::getInstance().centerStruct),  // Referência para centerStruct
                             footBarSizes(&GlobalDisplay::getInstance().footBarStruct),
                             canvas(*GlobalDisplay::getInstance().getCanvas()),
                             bgColor(TFT_BLACK),
                             textColor(TFT_WHITE),
                             borderColor(TFT_WHITE) {
    GlobalDisplay::getInstance().footBarControl(true, 16);
}

Calculadora::~Calculadora() {
}

void Calculadora::onAppOpen() {
    if (xSemaphoreTake(onAppOpenSemaphore, portMAX_DELAY) == pdTRUE) {
        nloop = 800;

        ESP_LOGI(TAG, "Iniciando delay de 800");

        const uint8_t* icon = getIcon();
        if (icon) {
            printf("Ícone carregado com sucesso.\n");
        } else {
            printf("Erro ao carregar ícone.\n");
        }
        setOpened(true);
        xSemaphoreGive(onAppOpenSemaphore);
    }
}

void Calculadora::onAppTick() {
    // nloop -= 1;
    // if (nloop % 10 == 0) {
    //     ESP_LOGI(TAG, "Rodando");
    // }
    // if (nloop < 1) {
    //     AppManager::getInstance().closeApp("Calculadora");
    // }
}

void Calculadora::onAppClose() {
    ESP_LOGI(TAG, "Fechando Calculadora");
    nloop = 800;
    ESP_LOGI(TAG, "Abrindo Launcher");
    AppManager::getInstance().openApp("Launcher");
}

void Calculadora::draw() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();

    centerX0 = static_cast<int32_t>(centerSizes->x0);
    centerY0 = static_cast<int32_t>(centerSizes->y0);  // or wherever centerY0 is set
    centerX1 = static_cast<int32_t>(centerSizes->x1);
    centerY1 = static_cast<int32_t>(centerSizes->y1);
    centerWidth = static_cast<int32_t>(centerSizes->width);
    centerHeight = static_cast<int32_t>(centerSizes->height);

    footBarX0 = static_cast<int32_t>(footBarSizes->x0);
    footBarY0 = static_cast<int32_t>(footBarSizes->y0);  // or wherever footBarY0 is set
    footBarX1 = static_cast<int32_t>(footBarSizes->x1);
    footBarY1 = static_cast<int32_t>(footBarSizes->y1);
    footBarWidth = static_cast<int32_t>(footBarSizes->width);
    footBarHeight = static_cast<int32_t>(footBarSizes->height);
    int radius = 10;  // Radius for the rounded corners
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw the top horizontal line with rounded corners

        canvas.drawLine(centerX0, centerY0, centerX0 + centerWidth - 1, centerY0, borderColor);

        // Draw the vertical sides
        canvas.drawLine(centerX0, centerY0, centerX0, centerY0 + centerHeight - 1, borderColor);                                      // Left side
        canvas.drawLine(centerX0 + centerWidth - 1, centerY0, centerX0 + centerWidth - 1, centerY0 + centerHeight - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        canvas.drawLine(centerX0, centerY0 + centerHeight - 1, centerX0 + centerWidth - 1, centerY0 + centerHeight - 1, borderColor);

        // Draw the vertical sides
        canvas.drawLine(footBarX0, footBarY0, footBarX0, footBarY0 + footBarHeight - radius - 1, borderColor);                                        // Left side
        canvas.drawLine(footBarX0 + footBarWidth - 1, footBarY0, footBarX0 + footBarWidth - 1, footBarY0 + footBarHeight - radius - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        canvas.drawLine(footBarX0 + radius, footBarY0 + footBarHeight - 1, footBarX0 + footBarWidth - radius - 1, footBarY0 + footBarHeight - 1, borderColor);

        // Draw the bottom-left rounded corner
        canvas.drawCircleHelper(footBarX0 + radius, footBarY0 + footBarHeight - radius - 1, radius, 8, borderColor);

        // Draw the bottom-right rounded corner
        canvas.drawCircleHelper(footBarX0 + footBarWidth - radius - 1, footBarY0 + footBarHeight - radius - 1, radius, 4, borderColor);

        xSemaphoreGive(canvasSemaphore);
        // ESP_LOGI(TAG, "Imprimiu");
    }
}

const uint8_t* Calculadora::getIcon() {
    return calc_icon;
}

size_t Calculadora::getIconSize() {
    return sizeof(calc_icon);
}

void Calculadora::clearCenter() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        canvas.fillRect(centerX0, centerY0, centerWidth, centerHeight, bgColor);
        xSemaphoreGive(canvasSemaphore);
    }
}
void Calculadora::clearFootBar() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        canvas.fillRect(footBarX0, footBarY0, footBarWidth, footBarHeight, bgColor);
        xSemaphoreGive(canvasSemaphore);
    }
}