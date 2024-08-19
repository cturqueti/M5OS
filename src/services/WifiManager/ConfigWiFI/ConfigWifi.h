#ifndef CONFIGWIFI_H
#define CONFIGWIFI_H

#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <WiFi.h>

class ConfigWifi {
   public:
    ConfigWifi();
    void begin(const char* _preferencesAddress);
    // void handleClient();

   private:
    void startServer();
    void handleRoot(AsyncWebServerRequest* request);
    void handleConfig(AsyncWebServerRequest* request);
    void setupMDNS();
    void startAP();

    const char* preferencesAddress;
    Preferences preferences;
    MDNSResponder mdns;
    AsyncWebServer server;
    // DNSServer dnsServer;
};

#endif  // CONFIGWIFI_H