#include "WiFiManager.h"

#include "esp_log.h"

const char* WiFiManager::TAG = "WiFi";

bool WiFiManager::connected = false;

WiFiManager::WiFiManager() : priority(99), server(80), serviceOpenFiniched(false), dnsServerStarted(false) {}

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
        passwd = preferences.getString("passwd").c_str();
        preferences.end();
        connectToWiFi();
    } else {
        ESP_LOGI(TAG, "No Wi-Fi credentials found. Starting AP...");
        WiFi.softAP("ConfigWifi");
        IPAddress IP = WiFi.softAPIP();
        ESP_LOGI(TAG, "IP Address: %s", IP.toString().c_str());
        dnsServerStarted = dnsServer.start(53, "*", IP);
        serviceOpenFiniched = true;
        setupMDNS();
        startServer();

        preferences.end();
        // configWifiInstance.begin(TAG);
    }
}

void WiFiManager::connectToWiFi() {
    ESP_LOGV(TAG, "Connecting to Wi-Fi...");
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
        configWifiInstance.begin(TAG);
    }
}

void WiFiManager::onServiceTick() {
    if (millis() - lastMillis > 100 && dnsServerStarted == true) {
        ESP_LOGI(TAG, "Processando DNS");
        dnsServer.processNextRequest();
        // configWifiInstance.handleClient();
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
    server.on("/", HTTP_GET, std::bind(&WiFiManager::handleRoot, this, std::placeholders::_1));

    // Rota de configuração via POST
    server.on("/config", HTTP_POST, std::bind(&WiFiManager::handleConfig, this, std::placeholders::_1));

    // Iniciar o servidor
    server.begin();
    ESP_LOGI(TAG, "Servidor HTTP iniciado.");
}

void WiFiManager::handleRoot(AsyncWebServerRequest* request) {
    if (request == nullptr) {
        ESP_LOGE(TAG, "Request é nulo.");
        return;
    }
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

void WiFiManager::handleConfig(AsyncWebServerRequest* request) {
    if (request == nullptr) {
        ESP_LOGE(TAG, "Request é nulo.");
        return;
    }
    ESP_LOGI(TAG, "Requisitou /config");
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
        String ssid = request->getParam("ssid", true)->value();
        String password = request->getParam("password", true)->value();
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        preferences.end();

    } else {
        request->send(400, "text/plain", "Parâmetros ausentes");
        return;
    }

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

void WiFiManager::handleClient() {
    dnsServer.processNextRequest();
}