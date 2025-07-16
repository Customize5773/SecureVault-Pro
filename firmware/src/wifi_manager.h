#pragma once
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define AP_SSID "SecureVault_AP"
#define AP_PASS "StrongPass123!"
#define API_KEY "SECURE-KEY-9876"

class WiFiManager {
public:
    WiFiManager();
    void begin();
    String getIP();
    
private:
    AsyncWebServer server;
    void setupAP();
    void startServer();
};
