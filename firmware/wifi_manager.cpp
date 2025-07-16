#include "wifi_manager.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <esp_wpa2.h>

WiFiManager::WiFiManager() : server(443) {}

void WiFiManager::begin() {
    setupAP();
    startServer();
}

void WiFiManager::setupAP() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASS, 1, 0, 4);
    Serial.printf("AP Started: %s\n", AP_SSID);
}

void WiFiManager::startServer() {
    server.on("/set", HTTP_GET, [this](AsyncWebServerRequest *request){
        if(request->hasParam("key") && request->getParam("key")->value() == API_KEY) {
            request->send(200, "text/plain", "OK");
        } else {
            request->send(401, "text/plain", "Unauthorized");
        }
    });
    
    server.begin();
    MDNS.begin("securevault");
}

String WiFiManager::getIP() {
    return WiFi.softAPIP().toString();
}
