#ifndef CONFIGWIFI_H
#define CONFIGWIFI_H

#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

class ConfigWifi {
   public:
    ConfigWifi(const char* _preferencesAddress);
    void begin();
    void handleClient();

   private:
    void startServer();
    void handleRoot();
    void handleConfig();
    void setupMDNS();
    void startAP();

    const char* preferencesAddress;
    Preferences preferences;
    MDNSResponder mdns;
    WebServer server;  // Mudança de AsyncWebServer para WebServer
    DNSServer dnsServer;
};

#endif  // CONFIGWIFI_H