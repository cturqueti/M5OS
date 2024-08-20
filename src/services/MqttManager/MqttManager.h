#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include <PubSubClient.h>

#include "../WifiManager/WiFiManager.h"
#include "MqttConnected.h"
#include "MqttDisconnected.h"

struct TopicParts {
    std::string part1;
    std::string part2;
    bool success;
};

class MqttManager : public Service {
   public:
    static MqttManager& getInstance() {
        static MqttManager instance;
        return instance;
    }

    MqttManager();
    virtual ~MqttManager();

    void onServiceOpen();
    void onServiceTick();
    void onServiceClose();
    void draw();
    int servicePriority() const { return priority; }
    inline bool isConnected() { return connected; }
    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   private:
    WiFiClient espClient;
    Preferences preferences;
    PubSubClient mqttClient;

    void reconnect();
    void callback(char* topic, byte* payload, unsigned int length);

    std::string removePrefix(const std::string& topic, const std::string& prefix);
    TopicParts splitTopic(const char* topic);
    void addLineToBuffer(const String& text);

    static std::string mqttUser;
    static std::string mqttPassword;
    const char* hostName = "modulo1";
    const char* mqttServiceName = "_mqtt";
    const char* protocol = "_tcp";

    std::string module = hostName;

    static bool connected;
    static bool disconnected;
    int lastMillis;
    uint8_t priority;
    static const char* TAG;
    IPAddress serverIp;
};

#endif  // WIFIMANAGER_H
