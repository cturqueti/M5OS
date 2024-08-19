#include "ConfigWifi.h"

#include "esp_log.h"

static const char *TAG = "ConfigWiFi";

ConfigWifi::ConfigWifi(const char *_preferencesAddress)
    : preferencesAddress(_preferencesAddress), server(80) {}

void ConfigWifi::begin() {
    if (!SPIFFS.begin(true)) {
        ESP_LOGI(TAG, "Failed to mount SPIFFS");
        return;
    }
    startAP();
    setupMDNS();
    startServer();
}

void ConfigWifi::startAP() {
    WiFi.mode(WIFI_AP);

    IPAddress local_IP(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    // Configurar o AP
    if (WiFi.softAPConfig(local_IP, gateway, subnet)) {
        // Iniciar o AP
        WiFi.softAP("ConfigWifi");

        IPAddress IP = WiFi.softAPIP();
        ESP_LOGI(TAG, "Access Point Started. IP Address: %s", IP.toString().c_str());
    } else {
        ESP_LOGE(TAG, "Não foi possível configurar o AP");
    }
}

void ConfigWifi::setupMDNS() {
    if (!mdns.begin("configwifi")) {
        ESP_LOGE(TAG, "Failed to start mDNS");
    } else {
        ESP_LOGI(TAG, "mDNS started. Access http://configwifi.local");
    }
}

void ConfigWifi::startServer() {
    // Rota principal, serve o arquivo HTML da configuração
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/config.html", "text/html");
    });

    // Ignorar favicon.ico para evitar logs desnecessários
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(204);  // Resposta vazia, sem conteúdo
    });

    // Rota de configuração via POST
    server.on("/config", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
            String ssid = request->getParam("ssid", true)->value();
            String password = request->getParam("password", true)->value();

            this->preferences.begin(this->preferencesAddress);
            this->preferences.putString("ssid", ssid);
            this->preferences.putString("password", password);
            this->preferences.end();

            // Responder ao cliente
            request->send(200, "text/plain", "Configurações salvas, reiniciando...");

            // Reiniciar ESP32 após enviar resposta
            delay(1000);
            ESP.restart();
        } else {
            request->send(400, "text/plain", "Parâmetros inválidos");
        }
    });

    // Iniciar o servidor
    server.begin();
    ESP_LOGI(TAG, "Servidor HTTP iniciado.");
}
