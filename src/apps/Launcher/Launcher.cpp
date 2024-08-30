#include "Launcher.h"

#include "esp_log.h"

const char* Launcher::TAG = "Launcher";

Launcher::Launcher() : centerWindow(GlobalDisplay::getInstance().centerStruct),
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
    canvas.fillRect(centerWindow.x0 + 5, centerWindow.y0 + 5, centerWindow.width - 5, centerWindow.height - 5, bgColor);

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
        if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
            canvas.setTextSize(2);
            canvas.setTextColor(WHITE);
            canvas.drawCenterString(apps[selectIndex], 120, 110);
            String iconPath = "/icons/" + apps[selectIndex] + ".png";
            int x = ((240 - 75) / 2);
            int y2 = ((135 - 75) / 2);

            auto app = AppManager::getInstance().getApp(apps[selectIndex].c_str());
            if (app->getIconSize() != 0 && app->getIcon() != nullptr) {
                ESP_LOGI(TAG, "Tamanho do ícone: %d", app->getIconSize());
                // Talvez adicionar um teste para ver se o primeiro byte do ícone não é nulo:
                ESP_LOGI(TAG, "Primeiro byte do ícone: 0x%02X", app->getIcon()[0]);
                canvas.drawPng(app->getIcon(), app->getIconSize(), x, y2, 75, 75);
                ESP_LOGI(TAG, "Ícone desenhado");

            } else {
                ESP_LOGI(TAG, "Ícone não encontrado ou vazio");
                canvas.drawPng(m5os, 435, x + 5, y2 + 5, 75, 75, 0, 0, 2, 2);
                ESP_LOGI(TAG, "Ícone padrão desenhado");
            }
            canvas.pushSprite(0, 0);
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
    int centerX = centerWindow.width / 2;
    int centerY = centerWindow.height / 2;

    int radius = 10;  // Radius for the rounded corners
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        // Draw the top horizontal line with rounded corners

        canvas.drawLine(centerWindow.x0, centerWindow.y0, centerWindow.x0 + centerWindow.width - 1, centerWindow.y0, borderColor);

        // Draw the vertical sides
        canvas.drawLine(centerWindow.x0, centerWindow.y0, centerWindow.x0, centerWindow.y0 + centerWindow.height - radius - 1, borderColor);                                                    // Left side
        canvas.drawLine(centerWindow.x0 + centerWindow.width - 1, centerWindow.y0, centerWindow.x0 + centerWindow.width - 1, centerWindow.y0 + centerWindow.height - radius - 1, borderColor);  // Right side

        // Draw the bottom horizontal line
        canvas.drawLine(centerWindow.x0 + radius, centerWindow.y0 + centerWindow.height - 1, centerWindow.x0 + centerWindow.width - radius - 1, centerWindow.y0 + centerWindow.height - 1, borderColor);

        // Draw the bottom-left rounded corner
        canvas.drawCircleHelper(centerWindow.x0 + radius, centerWindow.y0 + centerWindow.height - radius - 1, radius, 8, borderColor);

        // Draw the bottom-right rounded corner
        canvas.drawCircleHelper(centerWindow.x0 + centerWindow.width - radius - 1, centerWindow.y0 + centerWindow.height - radius - 1, radius, 4, borderColor);

        // Draw the text
        canvas.setTextColor(textColor);
        canvas.setTextSize(2);
        canvas.setCursor(centerWindow.x0 + 5, centerWindow.y0 + (centerWindow.height - 1) / 2);
        canvas.print("<");
        int textWidth = canvas.textWidth(">");
        canvas.setCursor(centerWindow.x1 - textWidth - 2, centerWindow.y0 + (centerWindow.height - 1) / 2);
        canvas.print(">");

        xSemaphoreGive(canvasSemaphore);
        // ESP_LOGI(TAG, "Imprimiu");
    }
}

void Launcher::clearCenter() {
    SemaphoreHandle_t canvasSemaphore = GlobalDisplay::getInstance().getSemaphore();
    if (xSemaphoreTake(canvasSemaphore, portMAX_DELAY) == pdTRUE) {
        canvas.fillRect(centerWindow.x0, centerWindow.y0, centerWindow.width, centerWindow.height, bgColor);
        xSemaphoreGive(canvasSemaphore);
    }
}