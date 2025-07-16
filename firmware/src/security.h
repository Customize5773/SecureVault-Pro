#pragma once
#include <Preferences.h>

#define CODE_LENGTH 4
#define MAX_ATTEMPTS 5
#define LOCKOUT_TIME 300000  // 5 minutes
const int DEFAULT_CODE[CODE_LENGTH] = {1, 2, 3, 4};

class Security {
public:
    Security();
    void begin();
    bool validateCode(int* entered);
    void updateCode(int* newCode);
    bool isLockedOut();
    
private:
    Preferences prefs;
    int correctCode[CODE_LENGTH];
    int failedAttempts;
    unsigned long lastAttempt;
    void loadCode();
};
