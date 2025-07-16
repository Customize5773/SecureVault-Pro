#include "servo_control.h"
#include <Preferences.h>

ServoControl::ServoControl(int pin) : 
    servoPin(pin), 
    currentPosition(0),
    targetPosition(0),
    lastUpdate(0) {}

void ServoControl::begin() {
    servo.attach(servoPin);
    loadState();
    moveTo(targetPosition);
}

void ServoControl::unlock() {
    targetPosition = 90;
    saveState();
}

void ServoControl::lock() {
    targetPosition = 0;
    saveState();
}

void ServoControl::update() {
    if(millis() - lastUpdate > SERVO_UPDATE_INTERVAL) {
        if(currentPosition != targetPosition) {
            currentPosition += (targetPosition > currentPosition) ? 1 : -1;
            servo.write(currentPosition);
        }
        lastUpdate = millis();
    }
}

void ServoControl::saveState() {
    Preferences prefs;
    prefs.begin("servo", false);
    prefs.putInt("position", targetPosition);
    prefs.end();
}

void ServoControl::loadState() {
    Preferences prefs;
    prefs.begin("servo", true);
    targetPosition = prefs.getInt("position", 0);
    prefs.end();
    servo.write(targetPosition);
    currentPosition = targetPosition;
}

bool ServoControl::isUnlocked() {
    return targetPosition == 90;
}
