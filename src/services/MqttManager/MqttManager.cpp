#include "MqttManager.h"

#include "esp_log.h"

const char* MqttManager::TAG = "MQTT";

bool MqttManager::connected = false;
bool MqttManager::disconnected = false;
std::string MqttManager::mqttUser = "";
std::string MqttManager::mqttPassword = "";

MqttManager::MqttManager() : priority(99),
                             lastMillis(0) {
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
        mqttClient.setCredentials(mqttUser.c_str(), mqttPassword.c_str());
        mqttClient.setServer(mqttServiceName, protocol);
        mqttClient.connect();
    }
    if (WiFiManager::getInstance().isConnected()) {
        ESP_LOGI(TAG, "Wi-Fi is connected, starting MQTT...");

        connected = true;
    } else {
        ESP_LOGW(TAG, "Wi-Fi is not connected, cannot start MQTT.");
        connected = false;
    }
}

void MqttManager::onServiceTick() {
    if (millis() - lastMillis > 100) {
        if (!connected) {
            if (WiFiManager::getInstance().isConnected()) {
                ESP_LOGI(TAG, "Wi-Fi is now connected, trying to start MQTT...");

                connected = true;
            }
        }
        // while (disconnected) {
        //     vTaskDelay(pdMS_TO_TICKS(50));
        // }

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

void MqttManager::onMqttConnect(bool sessionPresent) {
    ESP_LOGI(TAG, "Connected to MQTT.");
    ESP_LOGI(TAG, "Session present: %d", sessionPresent ? "true" : "false");

    // Subscribing to topics, resubscribe if sessionPresent is false
    if (!sessionPresent) {
        uint16_t packetIdSub = mqttClient.subscribe((module + "/#").c_str(), 2);
        ESP_LOGI(TAG, "Subscribing to %s/# at QoS 2, packetId: %s", module.c_str(), String(packetIdSub).c_str());

        packetIdSub = mqttClient.subscribe("broadcast/#", 2);
        ESP_LOGI(TAG, "Subscribing to broadcast/# at QoS 2, packetId: %s", String(packetIdSub).c_str());
    }
}

void MqttManager::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    ESP_LOGW(TAG, "Disconnected from MQTT.");

    if (WiFiManager::getInstance().isConnected()) {
        vTaskDelay(pdMS_TO_TICKS(50));
        mqttReconnectTimer();
    }
}

void MqttManager::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    ESP_LOGI(TAG, "Subscribe acknowledged.");
    ESP_LOGI(TAG, "  packetId: %s", String(packetId).c_str());
    ESP_LOGI(TAG, "  qos: %s", String(qos).c_str());
}

void MqttManager::onMqttUnsubscribe(uint16_t packetId) {
    ESP_LOGI(TAG, "Unsubscribe acknowledged.");
    ESP_LOGI(TAG, "  packetId: %s", String(packetId).c_str());
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
