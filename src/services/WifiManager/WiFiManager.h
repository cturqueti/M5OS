#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>

#include "ConfigWiFI/ConfigWifi.h"
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
    WiFiManager(WiFiManager const&) = delete;
    void operator=(WiFiManager const&) = delete;

    // void startAP();
    void connectToWiFi();

    static void WiFiEvent(WiFiEvent_t event);

    std::string ssid;
    std::string passwd;
    Preferences preferences;
    // DNSServer dnsServer;
    WebServer server;
    ConfigWifi configWifiInstance;
    static bool connected;
    int lastMillis;
    uint8_t priority;
    static const char* TAG;
};

#endif  // WIFIMANAGER_H
