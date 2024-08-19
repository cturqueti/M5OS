#include "WiFiManager.h"

#include "esp_log.h"

const char* WiFiManager::TAG = "WiFi";

bool WiFiManager::connected = false;

WiFiManager::WiFiManager() : priority(99), configWifiInstance(TAG) {}

WiFiManager::~WiFiManager() {}

void WiFiManager::onServiceOpen() {
    if (!preferences.begin(TAG, false)) {
        ESP_LOGE(TAG, "Failed to initialize Preferences");
        return;
    }

    if (preferences.isKey("ssid")) {
        ssid = preferences.getString("ssid").c_str();
        passwd = preferences.getString("passwd").c_str();
        preferences.end();
        connectToWiFi();
    } else {
        preferences.end();
        ESP_LOGI(TAG, "No Wi-Fi credentials found. Starting AP...");

        configWifiInstance.begin();
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
        configWifiInstance.begin();
    }
}

void WiFiManager::onServiceTick() {
    if (millis() - lastMillis > 100) {
        // dnsServer.processNextRequest();
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
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            connected = true;
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            connected = false;
            break;
    }
}

// void WiFiManager::startAP() {
//     WiFi.mode(WIFI_AP);
//     WiFi.softAP("ConfigWifi");
//     IPAddress IP = WiFi.softAPIP();
//     ESP_LOGI(TAG, "Access Point Started. IP Address: %s", IP.toString().c_str());
//     dnsServer.start(53, "*", IP);
// }