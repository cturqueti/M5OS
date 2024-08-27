#include "Launcher.h"

#include "esp_log.h"

const char* Launcher::TAG = "Launcher";

Launcher::Launcher() : x0(GlobalDisplay::getInstance().centerStruct.x0),
                       y0(GlobalDisplay::getInstance().centerStruct.y0),
                       x1(GlobalDisplay::getInstance().centerStruct.x1),
                       y1(GlobalDisplay::getInstance().centerStruct.y1),
                       width(GlobalDisplay::getInstance().centerStruct.width),
                       height(GlobalDisplay::getInstance().centerStruct.height),
                       center(*GlobalDisplay::getInstance().getCanvas()),
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
    if (apps.empty()) {
        center.setTextSize(2);
        center.setTextColor(textColor);
        center.drawCenterString("No apps found", 120, 66);
        needRedraw = false;
        ESP_LOGI(TAG, "Não foi encontrado APPs");
        return;
    } else {
        ESP_LOGI(TAG, "Foi encontrado APPs");
    }
    nloop -= 1;
    if (nloop % 10 == 0) {
        ESP_LOGI(TAG, "Rodando");
    }
    // ESP_LOGI(TAG, "Fazendo algo no Launcher");
    if (nloop < 1) {
        setClosed(true);
    }
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
    int centerX = width / 2;
    int centerY = height / 2;
    int radius = 10;  // Radius for the rounded corners
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw the top horizontal line with rounded corners
        center.drawRect(x0, y0, width, height, bgColor);
        center.drawLine(x0, y0, x0 + width - 1, y0, borderColor);

        // Draw the vertical sides
        center.drawLine(x0, y0, x0, y0 + height - radius - 1, borderColor);                          // Left side
        center.drawLine(x0 + width - 1, y0, x0 + width - 1, y0 + height - radius - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        center.drawLine(x0 + radius, y0 + height - 1, x0 + width - radius - 1, y0 + height - 1, borderColor);

        // Draw the bottom-left rounded corner
        center.drawCircleHelper(x0 + radius, y0 + height - radius - 1, radius, 8, borderColor);

        // Draw the bottom-right rounded corner
        center.drawCircleHelper(x0 + width - radius - 1, y0 + height - radius - 1, radius, 4, borderColor);

        // Draw the text
        center.setTextColor(textColor);
        center.setTextSize(2);
        center.setCursor(x0 + 5, y0 + (height - 1) / 2);
        center.print("<");
        int textWidth = center.textWidth(">");
        center.setCursor(x1 - textWidth - 2, y0 + (height - 1) / 2);
        center.print(">");

        xSemaphoreGive(canvasSemaphore);
        // ESP_LOGI(TAG, "Imprimiu");
    }
}
