#include "WiFiManager.h"

#include "esp_log.h"

const char* WiFiManager::TAG = "WiFi";

bool WiFiManager::connected = false;
bool WiFiManager::disconnected = false;

WiFiManager::WiFiManager() : priority(99),
                             server(80),  // porta do servidor: 80
                             serverConfigWiFiStarted(false),
                             lastMillis(0) {}

WiFiManager::~WiFiManager() {}

// -------------------- Functions common -------------------- //
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

void WiFiManager::onServiceTick() {
    if (millis() - lastMillis > 100) {
        if (serverConfigWiFiStarted) {
            server.handleClient();
        }
        while (disconnected) {
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        lastMillis = millis();
    }
}

void WiFiManager::onServiceClose() {
    if (isConnected()) {
        ESP_LOGI(TAG, "Wi-Fi disconnected.");
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        connected = false;
        disconnected = true;
    }
}

void WiFiManager::draw() {}

const uint8_t* WiFiManager::getIcon() {
    return connected ? wifiConnectedIcon : wifiDisconnectedIcon;
}

size_t WiFiManager::getIconSize() {
    return connected ? sizeof(wifiConnectedIcon) : sizeof(wifiDisconnectedIcon);
}

// ------------------ WifiManager functions ------------------ //
void WiFiManager::connectToWiFi() {
    ESP_LOGV(TAG, "Connecting to Wi-Fi...");
    ESP_LOGV(TAG, "SSID: %s", ssid.c_str());
    WiFi.mode(WIFI_STA);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        this->WiFiEvent(event);
    });

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
        connected = true;
    } else {
        ESP_LOGW(TAG, "Failed to connect to Wi-Fi.");
        WiFi.disconnect(true);  // Desconecta antes de tentar se reconectar
    }
}

void WiFiManager::WiFiEvent(WiFiEvent_t event) {
    if (event == ARDUINO_EVENT_WIFI_SCAN_DONE) {
        ESP_LOGI(TAG, "Wi-Fi scan done");

    } else if (event == ARDUINO_EVENT_WIFI_STA_START) {
        ESP_LOGI(TAG, "Wi-Fi STA started");

    } else if (event == ARDUINO_EVENT_WIFI_STA_STOP) {
        ESP_LOGI(TAG, "Wi-Fi STA stopped");

    } else if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi STA connected");
        connected = true;
        disconnected = false;

    } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi STA disconnected");
        connected = false;
        disconnected = true;
        // Tentar reconectar
        onServiceOpen();

    } else if (event == ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE) {
        ESP_LOGI(TAG, "Wi-Fi authentication mode changed");

    } else if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        ESP_LOGI(TAG, "Got IP Address: %s", WiFi.localIP().toString().c_str());

    } else if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP6) {
        ESP_LOGI(TAG, "Got IPv6 Address");

    } else if (event == ARDUINO_EVENT_WIFI_STA_LOST_IP) {
        ESP_LOGI(TAG, "Lost IP Address");

    } else if (event == ARDUINO_EVENT_WIFI_AP_START) {
        ESP_LOGI(TAG, "Wi-Fi AP started");

    } else if (event == ARDUINO_EVENT_WIFI_AP_STOP) {
        ESP_LOGI(TAG, "Wi-Fi AP stopped");

    } else if (event == ARDUINO_EVENT_WIFI_AP_STACONNECTED) {
        ESP_LOGI(TAG, "A client connected to the AP");

    } else if (event == ARDUINO_EVENT_WIFI_AP_STADISCONNECTED) {
        ESP_LOGI(TAG, "A client disconnected from the AP");

    } else if (event == ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED) {
        ESP_LOGI(TAG, "IP assigned to a client");

    } else if (event == ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED) {
        ESP_LOGI(TAG, "Probe request received");

    } else if (event == ARDUINO_EVENT_WIFI_AP_GOT_IP6) {
        ESP_LOGI(TAG, "AP got IPv6 Address");

    } else {
        ESP_LOGI(TAG, "Unhandled Wi-Fi event: %d", event);
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
    server.on("/", HTTP_GET, std::bind(&WiFiManager::handleRoot, this));

    server.on("/get_ssids", HTTP_GET, std::bind(&WiFiManager::handleGetSSIDs, this));

    server.on("/config", HTTP_POST, std::bind(&WiFiManager::handleConfig, this));

    server.begin();
    ESP_LOGI(TAG, "HTTP server started.");
    serverConfigWiFiStarted = true;
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

void WiFiManager::handleGetSSIDs() {
    int n = WiFi.scanNetworks();
    std::string options = "";

    for (int i = 0; i < n; ++i) {
        String ssid = WiFi.SSID(i);
        std::string ssid_str = ssid.c_str();

        // Escapar caracteres especiais para HTML
        std::string escaped_ssid = escapeHtml(ssid_str);

        options += "<option value=\"" + escaped_ssid + "\">" + escaped_ssid + "</option>";
    }

    server.send(200, "text/html", options.c_str());
}

void WiFiManager::handleConfig() {
    std::string ssid = server.arg("ssid").c_str();
    std::string password = server.arg("password").c_str();

    if (!ssid.empty() && !password.empty()) {
        File file = SPIFFS.open("/sucess.html", "r");
        if (!file) {
            ESP_LOGE(TAG, "Falha ao abrir sucess.html");
            server.send(500, "text/plain", "Erro interno do servidor");
            return;
        }

        // Salvar SSID e senha no Preferences
        preferences.begin(TAG, false);
        preferences.putString("ssid", ssid.c_str());
        preferences.putString("password", password.c_str());
        preferences.end();

        ESP_LOGI(TAG, "Servindo sucess.html para o cliente");
        // Redirecionar para a página de sucesso
        server.streamFile(file, "text/html");
        file.close();
        ESP.restart();
    } else {
        // Resposta caso falte algum campo
        server.send(400, "text/plain", "Erro: SSID e Senha são obrigatórios!");
    }
}

std::string WiFiManager::escapeHtml(const std::string& input) {
    std::string escaped = input;
    const std::pair<std::string, std::string> replacements[] = {
        {"\"", "&quot;"},
        {"&", "&amp;"},
        {"<", "&lt;"},
        {">", "&gt;"}};

    for (const auto& [findStr, replaceStr] : replacements) {
        size_t pos = 0;
        while ((pos = escaped.find(findStr, pos)) != std::string::npos) {
            escaped.replace(pos, findStr.length(), replaceStr);
            pos += replaceStr.length();
        }
    }

    return escaped;
}