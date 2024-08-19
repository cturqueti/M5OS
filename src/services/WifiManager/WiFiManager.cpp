#include "WiFiManager.h"

#include "esp_log.h"

const char* WiFiManager::TAG = "WiFi";

bool WiFiManager::connected = false;

WiFiManager::WiFiManager() : priority(99), server(80), serverStarted(false) {}

WiFiManager::~WiFiManager() {}

void WiFiManager::onServiceOpen() {
    if (!preferences.begin(TAG, false)) {
        ESP_LOGE(TAG, "Failed to initialize Preferences");
        return;
    }
    if (!SPIFFS.begin(true)) {
        ESP_LOGI(TAG, "Failed to mount SPIFFS");
        return;
    }

    if (preferences.isKey("ssid")) {
        ssid = preferences.getString("ssid").c_str();
        passwd = preferences.getString("password").c_str();
        preferences.end();
        connectToWiFi();
    } else {
        preferences.end();
        ESP_LOGI(TAG, "No Wi-Fi credentials found. Starting AP...");
        WiFi.softAP("ConfigWifi");
        IPAddress IP = WiFi.softAPIP();
        ESP_LOGI(TAG, "IP Address: %s", IP.toString().c_str());
        setupMDNS();
        startServer();
    }
}

void WiFiManager::connectToWiFi() {
    ESP_LOGV(TAG, "Connecting to Wi-Fi...");
    ESP_LOGV(TAG, "SSID: %s\tPassword: %s", ssid.c_str(), passwd.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), passwd.c_str());

    int attempts = 0;
    const int maxAttempts = 10;
    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        vTaskDelay(pdMS_TO_TICKS(500));
        attempts++;
    }

    if (attempts < maxAttempts) {
        ESP_LOGI(TAG, "Connected to Wi-Fi!");
        ESP_LOGI(TAG, "IP Address: %s", WiFi.localIP().toString().c_str());
        WiFi.onEvent(WiFiManager::WiFiEvent);
    } else {
        ESP_LOGW(TAG, "Failed to connect to Wi-Fi.");
    }
}

void WiFiManager::onServiceTick() {
    if (millis() - lastMillis > 100) {
        if (serverStarted) {
            server.handleClient();
        }

        lastMillis = millis();
    }
}

void WiFiManager::onServiceClose() {
    if (isConnected()) {
        ESP_LOGI(TAG, "Wi-Fi disconnected.");
    }
}

void WiFiManager::draw() {}

const uint8_t* WiFiManager::getIcon() {
    return connected ? wifiConnectedIcon : wifiDisconnectedIcon;
}

size_t WiFiManager::getIconSize() {
    return connected ? sizeof(wifiConnectedIcon) : sizeof(wifiDisconnectedIcon);
}

void WiFiManager::WiFiEvent(WiFiEvent_t event) {
    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        ESP_LOGI(TAG, "Got IP Address: %s", WiFi.localIP().toString().c_str());
        connected = true;
    } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected from Wi-Fi");
        connected = false;
    } else {
        ESP_LOGI(TAG, "Codigo: %d", event);
    }
}

void WiFiManager::setupMDNS() {
    if (!mdns.begin("configwifi")) {
        ESP_LOGE(TAG, "Failed to start mDNS");
    } else {
        ESP_LOGI(TAG, "mDNS started. Access http://configwifi.local");
    }
}

void WiFiManager::startServer() {
    // Rota principal, serve o arquivo HTML da configuração
    server.on("/", HTTP_GET, std::bind(&WiFiManager::handleRoot, this));

    // Rota de configuração via POST
    server.on("/config", HTTP_POST, std::bind(&WiFiManager::handleConfig, this));

    // Iniciar o servidor
    server.begin();
    ESP_LOGI(TAG, "Servidor HTTP iniciado.");
    serverStarted = true;
}

void WiFiManager::handleRoot() {
    ESP_LOGI(TAG, "Requisição recebida para /");
    File file = SPIFFS.open("/config.html", "r");
    if (!file) {
        ESP_LOGE(TAG, "Falha ao abrir config.html");
        server.send(500, "text/plain", "Erro interno do servidor");
        return;
    }

    ESP_LOGI(TAG, "Servindo config.html para o cliente");
    server.streamFile(file, "text/html");
    file.close();
}

void WiFiManager::handleConfig() {
    if (server.hasArg("ssid") && server.hasArg("password")) {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        File file = SPIFFS.open("/sucess.html", "r");
        if (!file) {
            ESP_LOGE(TAG, "Falha ao abrir sucess.html");
            server.send(500, "text/plain", "Erro interno do servidor");
            return;
        }

        // Salvar SSID e senha no Preferences
        preferences.begin(TAG, false);
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        preferences.end();

        ESP_LOGI(TAG, "Servindo sucess.html para o cliente");
        // Redirecionar para a página de sucesso
        server.streamFile(file, "text/html");
        file.close();
    } else {
        // Resposta caso falte algum campo
        server.send(400, "text/plain", "Erro: SSID e Senha são obrigatórios!");
    }
}
