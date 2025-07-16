#include "security.h"
#include <mbedtls/aes.h>

Security::Security() : prefs(), failedAttempts(0), lastAttempt(0) {}

void Security::begin() {
    prefs.begin("vault", false);
    loadCode();
    prefs.end();
}

bool Security::validateCode(int* entered) {
    if(millis() - lastAttempt < LOCKOUT_TIME) return false;
    
    lastAttempt = millis();
    bool match = true;
    
    for(int i = 0; i < CODE_LENGTH; i++) {
        if(entered[i] != correctCode[i]) {
            match = false;
            break;
        }
    }
    
    if(match) {
        failedAttempts = 0;
        return true;
    }
    
    if(++failedAttempts >= MAX_ATTEMPTS) {
        lastAttempt = millis();
    }
    return false;
}

void Security::updateCode(int* newCode) {
    prefs.begin("vault", false);
    for(int i = 0; i < CODE_LENGTH; i++) {
        correctCode[i] = newCode[i];
        prefs.putInt(("d" + String(i)).c_str(), newCode[i]);
    }
    prefs.end();
}

void Security::loadCode() {
    for(int i = 0; i < CODE_LENGTH; i++) {
        correctCode[i] = prefs.getInt(("d" + String(i)).c_str(), DEFAULT_CODE[i]);
    }
}

bool Security::isLockedOut() {
    return (millis() - lastAttempt) < LOCKOUT_TIME;
}
