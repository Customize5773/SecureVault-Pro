#include "ota_update.h"
#include <Update.h>

OTAUpdate::OTAUpdate(AsyncWebServer* srv) : server(srv) {}

void OTAUpdate::begin() {
    server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
    }, [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        handleUpdate(request, filename, index, data, len, final);
    });
}

void OTAUpdate::handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if(!index) {
        Serial.printf("Update Start: %s\n", filename.c_str());
        Update.begin(UPDATE_SIZE_UNKNOWN);
    }
    
    if(Update.write(data, len) != len) {
        Serial.println("Write Error");
    }
    
    if(final) {
        if(Update.end(true)) {
            Serial.println("Update Success");
            request->send(200, "text/plain", "OK");
            delay(1000);
            ESP.restart();
        } else {
            request->send(500, "text/plain", "FAIL");
        }
    }
}
