#include "ConfigWifi.h"

#include <SPIFFS.h>

#include "esp_log.h"

static const char* TAG = "ConfigWiFi";

ConfigWifi::ConfigWifi(const char* _preferencesAddress)
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
    // Rotas HTTP com WebServer
    server.on("/", HTTP_GET, std::bind(&ConfigWifi::handleRoot, this));
    server.on("/config", HTTP_POST, std::bind(&ConfigWifi::handleConfig, this));
    server.begin();
    ESP_LOGI(TAG, "HTTP server started");
}

void ConfigWifi::handleRoot() {
    File file = SPIFFS.open("/config.html", "r");
    if (!file) {
        server.send(404, "text/plain", "Page not found");
        ESP_LOGI(TAG, "Page not found");
        return;
    }

    String html = file.readString();
    file.close();

    server.send(200, "text/html", html);
    ESP_LOGI(TAG, "HTML page sent");
}

void ConfigWifi::handleConfig() {
    if (!server.hasArg("ssid") || !server.hasArg("passwd")) {
        server.send(400, "text/plain", "Invalid Request");
        return;
    }

    String ssid = server.arg("ssid");
    String passwd = server.arg("passwd");

    preferences.begin(preferencesAddress, false);
    preferences.putString("ssid", ssid);
    preferences.putString("passwd", passwd);
    preferences.end();

    File file = SPIFFS.open("/success.html", "r");
    if (!file) {
        server.send(404, "text/plain", "Page not found");
        ESP_LOGI(TAG, "Page not found");
        return;
    }

    String html = file.readString();
    file.close();
    server.send(200, "text/html", html);

    WiFi.softAPdisconnect(true);
    delay(1000);
    mdns.end();
    ESP.restart();
}

void ConfigWifi::handleClient() {
    // dnsServer.processNextRequest();
    server.handleClient();  // Processa as requisições do servidor Web
}
