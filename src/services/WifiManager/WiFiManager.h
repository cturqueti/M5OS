#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <WebServer.h>

#include "Service.h"
#include "WiFi.h"
#include "WifiConnected.h"
#include "WifiDisconnected.h"

class WiFiManager : public Service {
   public:
    static WiFiManager& getInstance() {
        static WiFiManager instance;
        return instance;
    }

    WiFiManager();
    virtual ~WiFiManager();

    void onServiceOpen();
    void onServiceTick();
    void onServiceClose();
    void draw();
    int servicePriority() const { return priority; }
    inline bool isConnected() { return connected; }
    const uint8_t* getIcon() override;
    size_t getIconSize() override;

   private:
    void connectToWiFi();

    void WiFiEvent(WiFiEvent_t event);
    void setupMDNS();
    void startServer();
    void handleRoot();
    void handleConfig();
    void reconnect();

    std::string ssid;
    std::string passwd;
    Preferences preferences;
    MDNSResponder mdns;
    WebServer server;
    static bool connected;
    static bool disconnected;
    int lastMillis;
    uint8_t priority;
    static const char* TAG;
    bool serverConfigWiFiStarted;
};

#endif  // WIFIMANAGER_H
