#pragma once
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class OTAUpdate {
public:
    OTAUpdate(AsyncWebServer* srv);
    void begin();
    
private:
    AsyncWebServer* server;
    void handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
};
