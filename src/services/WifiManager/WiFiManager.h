#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <SPIFFS.h>

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
    void handleClient();

   private:
    // WiFiManager(WiFiManager const&) = delete;
    // void operator=(WiFiManager const&) = delete;

    // void startAP();
    void connectToWiFi();

    static void WiFiEvent(WiFiEvent_t event);
    void setupMDNS();
    void startServer();
    void handleRoot(AsyncWebServerRequest* request);
    void handleConfig(AsyncWebServerRequest* request);

    std::string ssid;
    std::string passwd;
    Preferences preferences;
    MDNSResponder mdns;
    ConfigWifi configWifiInstance;
    AsyncWebServer server;
    DNSServer dnsServer;
    static bool connected;
    int lastMillis;
    uint8_t priority;
    static const char* TAG;
    bool serviceOpenFiniched;
    bool dnsServerStarted;
};

#endif  // WIFIMANAGER_H
