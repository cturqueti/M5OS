#include "MqttManager.h"

#include "esp_log.h"

const char* MqttManager::TAG = "MQTT";

bool MqttManager::connected = false;
bool MqttManager::disconnected = false;
std::string MqttManager::mqttUser = "";
std::string MqttManager::mqttPassword = "";

MqttManager::MqttManager() : priority(99),
                             lastMillis(0),
                             mqttClient(espClient),
                             serverIp(192, 168, 0, 72) {
    mqttClient.setServer(serverIp, 1883);
    mqttClient.setCallback(std::bind(&MqttManager::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

MqttManager::~MqttManager() {}

// -------------------- Functions common -------------------- //
void MqttManager::onServiceOpen() {
    if (!preferences.begin(TAG, false)) {
        ESP_LOGE(TAG, "Failed to initialize Preferences");
        return;
    }
    if (preferences.isKey("user")) {
        mqttUser = preferences.getString("user").c_str();
        mqttPassword = preferences.getString("password").c_str();
        preferences.end();
    } else {
        mqttUser = "sideout";
        mqttPassword = "sidetouch";
    }
    mqttClient.setServer(serverIp, 1883);

    if (WiFiManager::getInstance().isConnected()) {
        ESP_LOGI(TAG, "Wi-Fi is connected, starting MQTT...");
        reconnect();
        connected = true;
    } else {
        ESP_LOGW(TAG, "Wi-Fi is not connected, cannot start MQTT.");
        connected = false;
    }
}

void MqttManager::onServiceTick() {
    if (millis() - lastMillis > 100) {
        if (!mqttClient.connected()) {
            if (WiFiManager::getInstance().isConnected()) {
                ESP_LOGI(TAG, "Wi-Fi is now connected, trying to start MQTT...");
                reconnect();
                connected = true;
            }
        }
        lastMillis = millis();
    }
}

void MqttManager::onServiceClose() {
}

void MqttManager::draw() {}

const uint8_t* MqttManager::getIcon() {
    return connected ? mqttConnectedIcon : mqttDisconnectedIcon;
}

size_t MqttManager::getIconSize() {
    return connected ? sizeof(mqttConnectedIcon) : sizeof(mqttDisconnectedIcon);
}

// ------------------ WifiManager functions ------------------ //

void MqttManager::reconnect() {
    int attempts = 0;
    const int maxAttempts = 5;
    while (!mqttClient.connected() && attempts < maxAttempts) {
        ESP_LOGI(TAG, "[MQTT] Attempting connection... (attempt %d)", attempts + 1);
        ESP_LOGI(TAG, "Attempting MQTT connection...");
        if (mqttClient.connect("clientId", mqttUser.c_str(), mqttPassword.c_str())) {
            ESP_LOGI(TAG, "Connected to MQTT.");
            mqttClient.subscribe((module + "/#").c_str(), 2);
            mqttClient.subscribe("broadcast/#", 2);
            mqttClient.publish("teste", "Conectou bem");
        } else {
            ESP_LOGW(TAG, "Failed to connect to MQTT, rc=%d", mqttClient.state());
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        attempts++;
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    if (!mqttClient.connected()) {
        ESP_LOGE(TAG, "[MQTT] Failed to connect after %d attempts", attempts);
    }
}

void MqttManager::reconnect() {
    while (!mqttClient.connected()) {
        ESP_LOGI(TAG, "Attempting MQTT connection...");
        if (mqttClient.connect("clientId", mqttUser.c_str(), mqttPassword.c_str())) {
            ESP_LOGI(TAG, "Connected to MQTT.");
            mqttClient.subscribe((module + "/#").c_str(), 2);
            mqttClient.subscribe("broadcast/#", 2);
            mqttClient.publish("teste", "Conectou bem");
        } else {
            ESP_LOGW(TAG, "Failed to connect to MQTT, rc=%d", mqttClient.state());
            delay(5000);
        }
    }
}

void MqttManager::callback(char* topic, byte* payload, unsigned int length) {
    std::string topicStr(topic);
    std::string payloadStr((char*)payload, length);

    TopicParts parts = splitTopic(topic);

    if (parts.success) {
        ESP_LOGI(TAG, "Publish received.");
        ESP_LOGI(TAG, "  topic: %s", parts.part2.c_str());
        ESP_LOGI(TAG, "  len: %zu", length);
        ESP_LOGI(TAG, "  payload: %s", payloadStr.c_str());

        if (parts.part1 == module || parts.part1 == "broadcast") {
            if (parts.part2 == "read" && payloadStr == "ping") {
                ESP_LOGI(TAG, "terei que responder");
                mqttClient.publish((module + "/ip").c_str(), WiFi.localIP().toString().c_str(), true);
            }
        }
    }
}

std::string MqttManager::removePrefix(const std::string& topic, const std::string& prefix) {
    if (topic.find(prefix)) {
        return topic.substr(prefix.length());
    }
    return topic;
}

TopicParts MqttManager::splitTopic(const char* topic) {
    char topicCopy[128];
    strncpy(topicCopy, topic, sizeof(topicCopy) - 1);
    topicCopy[sizeof(topicCopy) - 1] = '\0';  // Certifique-se de que a string é terminada com nulo

    // Use strtok para separar a string
    char* part1 = strtok(topicCopy, "/");
    char* part2 = strtok(NULL, "/");

    TopicParts result;
    if (part1 != NULL && part2 != NULL) {
        result.part1 = std::string(part1);
        result.part2 = std::string(part2);
        result.success = true;
    } else {
        result.success = false;
    }
    return result;
}

void MqttManager::addLineToBuffer(const String& text) {
    // Move as linhas para cima para fazer espaço para a nova linha
    // for (int i = MAX_LINES - 1; i > 0; --i) {
    //     lineBuffer[i] = lineBuffer[i - 1];
    // }
    // // Adiciona o novo texto à primeira linha
    // lineBuffer[0] = text;
}
