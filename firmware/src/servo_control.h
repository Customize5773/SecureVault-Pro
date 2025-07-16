#pragma once
#include <ESP32Servo.h>

#define SERVO_UPDATE_INTERVAL 15

class ServoControl {
public:
    ServoControl(int pin);
    void begin();
    void unlock();
    void lock();
    void update();
    bool isUnlocked();
    
private:
    Servo servo;
    int servoPin;
    int currentPosition;
    int targetPosition;
    unsigned long lastUpdate;
    void saveState();
    void loadState();
};
