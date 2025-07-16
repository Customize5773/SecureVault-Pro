#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <Preferences.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Battery18650Stats.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_I2C_ADDR 0x3C

#define ENCODER_CLK 32
#define ENCODER_DT 19
#define ENCODER_SW 18
#define SERVO_PIN 14
#define TAMPER_PIN 27
#define BATTERY_PIN 34

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Preferences prefs;
Servo vaultServo;
Battery18650Stats battery(BATTERY_PIN);
AsyncWebServer server(80);

const int codeLength = 4;
int correctCode[codeLength] = {1, 1, 2, 2};
int enteredCode[codeLength] = {0};

int currentDigitValue = 0;
int currentIndex = 0;
int lastCLKState;
int failedAttempts = 0;
bool vaultUnlocked = false;
bool inMenu = false;
bool settingNewCode = false;
bool menuSelection = false;
bool readyToEnterNext = false;
bool tamperDetected = false;

const char* AP_SSID = "SecureVault_AP";
const char* AP_PASS = "StrongPass!";
const char* API_KEY = "SECRET-KEY-1234";

void IRAM_ATTR handleTamper() {
  tamperDetected = true;
}

void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  
  if (tamperDetected) {
    display.println("TAMPER DETECTED!");
    display.display();
    return;
  }

  if (!vaultUnlocked && !settingNewCode) {
    display.printf("PIN %d: %d", currentIndex + 1, currentDigitValue);
  } else if (settingNewCode) {
    display.printf("NEW PIN %d: %d", currentIndex + 1, currentDigitValue);
  } else if (vaultUnlocked && inMenu) {
    display.println(menuSelection ? "> Set PIN" : "> Lock");
    display.println(menuSelection ? "  Lock" : "  Set PIN");
  }
  
  display.printf("\nBatt: %.1fV", battery.getBatteryVolts());
  display.display();
}

void unlockVault() {
  vaultServo.write(90);
  vaultUnlocked = true;
  inMenu = true;
  failedAttempts = 0;
}

void lockVault() {
  vaultServo.write(0);
  vaultUnlocked = false;
  inMenu = false;
}

void handleSetCode(AsyncWebServerRequest *request) {
  if (!request->hasParam("key") || request->getParam("key")->value() != API_KEY) {
    request->send(401, "text/plain", "Invalid API Key");
    return;
  }

  if (request->hasParam("pin")) {
    String pinString = request->getParam("pin")->value();
    int newCode[4];
    int count = 0;
    
    for (int i = 0; i < pinString.length() && count < 4; i++) {
      if (isDigit(pinString[i])) {
        newCode[count++] = pinString[i] - '0';
      }
    }

    if (count == 4) {
      prefs.begin("vault", false);
      for (int i = 0; i < 4; i++) {
        prefs.putInt(String("d" + i).c_str(), newCode[i]);
        correctCode[i] = newCode[i];
      }
      prefs.end();
      request->send(200, "text/plain", "PIN Updated");
    } else {
      request->send(400, "text/plain", "Invalid PIN");
    }
  } else {
    request->send(400, "text/plain", "Missing PIN");
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);
  pinMode(TAMPER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TAMPER_PIN), handleTamper, FALLING);

  Wire.begin(21, 22);
  display.begin(SCREEN_I2C_ADDR, true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  
  vaultServo.attach(SERVO_PIN);
  lockVault();

  prefs.begin("vault", true);
  for (int i = 0; i < codeLength; i++) {
    correctCode[i] = prefs.getInt(String("d" + i).c_str(), correctCode[i]);
  }
  prefs.end();

  lastCLKState = digitalRead(ENCODER_CLK);
  showMessage("SecureVault Pro\nBooting...");

  WiFi.softAP(AP_SSID, AP_PASS);
  server.on("/set", HTTP_GET, handleSetCode);
  server.begin();

  battery.begin(4200, 3.7);
  updateDisplay();
}

void loop() {
  if (tamperDetected) {
    while(1) { 
      display.invertDisplay(true);
      delay(500);
      display.invertDisplay(false);
      delay(500);
    }
  }

  int newCLKState = digitalRead(ENCODER_CLK);
  if (lastCLKState != newCLKState && newCLKState == LOW) {
    int dtState = digitalRead(ENCODER_DT);
    currentDigitValue += (dtState == HIGH) ? 1 : -1;
    currentDigitValue = constrain(currentDigitValue, 0, 9);
    updateDisplay();
  }
  lastCLKState = newCLKState;

  if (digitalRead(ENCODER_SW) == LOW && !readyToEnterNext) {
    delay(300);
    readyToEnterNext = true;
    
    if (!vaultUnlocked && !settingNewCode) {
      enteredCode[currentIndex++] = currentDigitValue;
      currentDigitValue = 0;
      
      if (currentIndex >= codeLength) {
        bool match = true;
        for (int i = 0; i < codeLength; i++) {
          if (enteredCode[i] != correctCode[i]) match = false;
        }
        
        if (match) {
          unlockVault();
        } else if (++failedAttempts >= 5) {
          showMessage("LOCKED!\nWait 5 mins");
          delay(300000);
          failedAttempts = 0;
        } else {
          showMessage("Wrong PIN\nAttempts left:");
          display.printf("%d", 5 - failedAttempts);
          display.display();
          delay(2000);
        }
        currentIndex = 0;
      }
      updateDisplay();
      
    } else if (vaultUnlocked && inMenu) {
      if (menuSelection) {
        settingNewCode = true;
        vaultUnlocked = false;
        currentIndex = 0;
        showMessage("Set New PIN");
      } else {
        lockVault();
        showMessage("Vault Locked");
        delay(2000);
      }
      updateDisplay();
      
    } else if (settingNewCode) {
      correctCode[currentIndex++] = currentDigitValue;
      currentDigitValue = 0;
      
      if (currentIndex >= codeLength) {
        prefs.begin("vault", false);
        for (int i = 0; i < codeLength; i++) {
          prefs.putInt(String("d" + i).c_str(), correctCode[i]);
        }
        prefs.end();
        settingNewCode = false;
        lockVault();
        showMessage("New PIN Saved!");
        delay(2000);
      }
      updateDisplay();
    }
  }

  if (digitalRead(ENCODER_SW) == HIGH) {
    readyToEnterNext = false;
  }
}
