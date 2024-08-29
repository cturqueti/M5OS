#include "Launcher.h"

#include "esp_log.h"

const char* Launcher::TAG = "Launcher";

Launcher::Launcher() : centerSizes(&GlobalDisplay::getInstance().centerStruct),  // Referência para centerStruct
                       canvas(*GlobalDisplay::getInstance().getCanvas()),
                       lastMillis(millis()),
                       bgColor(TFT_BLACK),
                       textColor(TFT_WHITE),
                       borderColor(TFT_WHITE),
                       selectIndex(0),
                       needRedraw(true) {
}

Launcher::~Launcher() {
}

void Launcher::onAppOpen() {
    auto fetchedApps = AppManager::getInstance().listApps();
    apps.clear();
    for (const auto& app : fetchedApps) {
        ESP_LOGI(TAG, "App encontrado: %s", app.first.c_str());
        if (app.first == "AppLauncher") {
            continue;
        }
        apps.emplace_back(app.first.c_str());
    }
    selectIndex = 0;
    needRedraw = true;
    ESP_LOGI(TAG, "Iniciando delay de 800");
    printf("Iniciando Launcher.\n");
    nloop = 800;
    setOpened(true);
}

void Launcher::onAppTick() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();

    if (apps.empty()) {
        if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
            canvas.setTextSize(2);
            canvas.setTextColor(textColor);
            canvas.drawCenterString("No apps found", 120, 66);
            xSemaphoreGive(canvasSemaphore);
        }
        needRedraw = false;
        ESP_LOGI(TAG, "Não foi encontrado APPs");
        return;
    } else {
        String iconPath = "/icons/" + apps[selectIndex] + ".png";
        int x = ((240 - 75) / 2);
        int y2 = ((135 - 75) / 2);
        if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
            if (AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize() != 0) {
                canvas.drawPng(AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIcon(),
                               AppManager::getInstance().getApp(apps[selectIndex].c_str())->getIconSize(),
                               x, y2, 75, 75);
            } else {
                canvas.drawPng(m5os, 435, x + 5, y2 + 5, 75, 75, 0, 0, 2, 2);
            }
            xSemaphoreGive(canvasSemaphore);
        }
    }
    // nloop -= 1;
    // if (nloop % 10 == 0) {
    //     ESP_LOGI(TAG, "Rodando");
    // }
    // // ESP_LOGI(TAG, "Fazendo algo no Launcher");
    // if (nloop < 1) {
    //     setClosed(true);
    // }
}

void Launcher::onAppClose() {
    apps.clear();
    ESP_LOGI(TAG, "Fechando Launcher");
    nloop = 800;
    ESP_LOGI(TAG, "Abrindo Calculadora");
    AppManager::getInstance().openApp("Calculadora");
}

void Launcher::draw() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    int centerX = centerWidth / 2;
    int centerY = centerHeight / 2;

    centerX0 = static_cast<int32_t>(centerSizes->x0);
    centerY0 = static_cast<int32_t>(centerSizes->y0);  // or wherever centerY0 is set
    centerX1 = static_cast<int32_t>(centerSizes->x1);
    centerY1 = static_cast<int32_t>(centerSizes->y1);
    centerWidth = static_cast<int32_t>(centerSizes->width);
    centerHeight = static_cast<int32_t>(centerSizes->height);

    int radius = 10;  // Radius for the rounded corners
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw the top horizontal line with rounded corners
        canvas.drawRect(centerX0, centerY0, centerWidth, centerHeight, bgColor);
        canvas.drawLine(centerX0, centerY0, centerX0 + centerWidth - 1, centerY0, borderColor);

        // Draw the vertical sides
        canvas.drawLine(centerX0, centerY0, centerX0, centerY0 + centerHeight - radius - 1, borderColor);                                      // Left side
        canvas.drawLine(centerX0 + centerWidth - 1, centerY0, centerX0 + centerWidth - 1, centerY0 + centerHeight - radius - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        canvas.drawLine(centerX0 + radius, centerY0 + centerHeight - 1, centerX0 + centerWidth - radius - 1, centerY0 + centerHeight - 1, borderColor);

        // Draw the bottom-left rounded corner
        canvas.drawCircleHelper(centerX0 + radius, centerY0 + centerHeight - radius - 1, radius, 8, borderColor);

        // Draw the bottom-right rounded corner
        canvas.drawCircleHelper(centerX0 + centerWidth - radius - 1, centerY0 + centerHeight - radius - 1, radius, 4, borderColor);

        // Draw the text
        canvas.setTextColor(textColor);
        canvas.setTextSize(2);
        canvas.setCursor(centerX0 + 5, centerY0 + (centerHeight - 1) / 2);
        canvas.print("<");
        int textWidth = canvas.textWidth(">");
        canvas.setCursor(centerX1 - textWidth - 2, centerY0 + (centerHeight - 1) / 2);
        canvas.print(">");

        xSemaphoreGive(canvasSemaphore);
        // ESP_LOGI(TAG, "Imprimiu");
    }
}

void Launcher::clearCenter() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        canvas.fillRect(centerX0, centerY0, centerWidth, centerHeight, bgColor);
        xSemaphoreGive(canvasSemaphore);
    }
}