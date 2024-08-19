#include "ConfigWifi.h"

#include "esp_log.h"

static const char *TAG = "ConfigWiFi";

ConfigWifi::ConfigWifi()
    : server(80) {}

void ConfigWifi::begin(const char *_preferencesAddress) {
    preferencesAddress = _preferencesAddress;
    if (!SPIFFS.begin(true)) {
        ESP_LOGI(TAG, "Failed to mount SPIFFS");
        return;
    }
    startAP();
    setupMDNS();
    startServer();
}

void ConfigWifi::startAP() {
    // WiFi.mode(WIFI_AP);

    WiFi.softAP("ConfigWifi");
    IPAddress IP = WiFi.softAPIP();
    ESP_LOGI(TAG, "Access Point Started");
    ESP_LOGI(TAG, "IP Address: %s", IP.toString().c_str());
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
    server.on("/", HTTP_GET, std::bind(&ConfigWifi::handleRoot, this, std::placeholders::_1));

    // Rota de configuração via POST
    server.on("/config", HTTP_POST, std::bind(&ConfigWifi::handleConfig, this, std::placeholders::_1));

    // Iniciar o servidor
    server.begin();
    ESP_LOGI(TAG, "Servidor HTTP iniciado.");
}

void ConfigWifi::handleRoot(AsyncWebServerRequest *request) {
    // Carrega a página HTML do SPIFFS
    ESP_LOGI(TAG, "Requisitou /");
    String htmlPath = "/config.html";  // Caminho para seu arquivo HTML no SPIFFS

    // Lê o conteúdo do arquivo HTML
    File file = SPIFFS.open(htmlPath, "r");
    if (!file) {
        request->send(404, "text/plain", "Página não encontrada");
        ESP_LOGW(TAG, "Página não encontrada");
        return;
    }

    // Envia o conteúdo do arquivo HTML como resposta
    String html = file.readString();
    file.close();

    request->send(200, "text/html", html);
    ESP_LOGI(TAG, "Página HTML enviada");
}

void ConfigWifi::handleConfig(AsyncWebServerRequest *request) {
    ESP_LOGI(TAG, "Requisitou /config");
    String ssid = request->getParam("ssid", true)->value();
    String password = request->getParam("password", true)->value();

    // Salve o SSID e senha recebidos em uma memória não volátil
    preferences.begin(preferencesAddress, false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();

    String htmlPath = "/sucess.html";
    // Lê o conteúdo do arquivo HTML
    File file = SPIFFS.open(htmlPath, "r");
    if (!file) {
        request->send(404, "text/plain", "Página não encontrada");
        ESP_LOGW(TAG, "Página não encontrada");
        return;
    }

    // Envia o conteúdo do arquivo HTML como resposta
    String html = file.readString();
    file.close();

    request->send(200, "text/html", html);

    // Desconecte o AP e reinicie o ESP para conectar-se à nova rede
    WiFi.softAPdisconnect(true);
    delay(1000);
    mdns.end();
    ESP.restart();
}
