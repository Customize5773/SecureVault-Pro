### SecureVault Pro - Getting Started Guide  
**Version 2.1 | Last Updated: July 2025**  

---

### Quick Setup (3 Minutes)  
1. **Flash Firmware**  
   ```bash
   # Install PlatformIO
   pip install platformio
   
   # Build and upload
   pio run -t upload
   ```

2. **First Boot**  
   - Connect phone/laptop to WiFi: `SecureVault_AP` | Pass: `StrongPass!`  
   - Open browser: `https://192.168.4.1` (ignore security warning)  
   - Set initial PIN: `1 2 3 4`  

3. **Physical Setup**  
   - Insert battery → Connect USB power  
   - Turn rotary encoder to enter PIN  
   - Press to unlock!  

---

### Full Hardware Assembly  
**Tools Needed:** Soldering iron, wire cutters, Phillips screwdriver  

1. **Power System**  
   ```markdown
   - Connect TP4056 OUT+ to MT3608 IN+  
   - Solder 18650 battery to TP4056 BAT+/-  
   - MT3608 OUT+ → ESP32 5V pin  
   - MT3608 OUT- → Common ground  
   ```

2. **Main Board Connections**  
   | ESP32-S3 Pin | Component        | Wire Color |
   |--------------|------------------|------------|
   | GPIO21       | OLED SDA         | Blue       |
   | GPIO22       | OLED SCL         | Yellow     |
   | GPIO32       | Encoder CLK      | Green      |
   | GPIO19       | Encoder DT       | Orange     |
   | GPIO18       | Encoder SW       | Purple     |
   | GPIO14       | Servo Signal     | Red        |
   | GPIO27       | Vibration Sensor | Brown      |
   | GPIO34       | Battery Voltage  | White      |

3. **Mechanical Installation**  
   - Mount servo using M2 screws  
   - Glue reed switch inside lid edge  
   - Position magnet on main enclosure  
   - Secure vibration sensor with double-sided tape  

> **Pro Tip:** Test servo range before final assembly!  
> ```cpp
> void setup() { 
>   servo.write(0); delay(2000);
>   servo.write(90);  // Should unlock
> }
> ```

---

### Software Configuration  

#### 1. Set Up Development Environment  
```bash
# Clone repository
git clone https://github.com/Customize5773/SecureVault-Pro.git
cd SecureVault-Pro

# Install dependencies
pio lib install
```

#### 2. Configure WiFi (Two Options)  

**Option A: Web Portal (First Boot)**  
1. Connect to `SecureVault_AP`  
2. Visit `https://securevault.local`  
3. Enter home WiFi credentials  

**Option B: Config File**  
```cpp
// firmware/src/wifi_config.h
const char* HOME_SSID = "YourNetwork";
const char* HOME_PASS = "YourPassword";
```

#### 3. Security Customization  
```cpp
// firmware/src/security.h
#define API_KEY "ChangeThis!"         // Web access key
#define MAX_ATTEMPTS 3                // Lock after 3 fails
#define LOCKOUT_TIME 600000           // 10 minute lockout
```

---

### First Unlock Sequence  
1. **Rotary Control**  
   - Turn → Select digit (0-9)  
   - Press → Confirm digit  
   - Repeat for 4 digits  

2. **Successful Unlock**  
   - Servo moves 90°  
   - OLED shows: `> Lock` `  Set PIN`  

3. **Relock**  
   - Select `> Lock`  
   - Press encoder button  

---

### Advanced Features  
1. **Remote Unlock**  
   ```bash
   curl "https://securevault.local/set?key=API_KEY&pin=5678"
   ```

2. **Tamper Detection**  
   - Triggers if moved while locked  
   - OLED flashes inverted display  
   - Permanent lock until power cycle  

3. **Battery Monitoring**  
   - Voltage shown on OLED  
   - Critical level: <3.3V → Forces lock  

---

### Troubleshooting  
| Symptom               | Solution                          |
|-----------------------|-----------------------------------|
| No OLED display       | Check I2C address (0x3C vs 0x3D) |
| Servo jitters         | Add 100µF capacitor across power |
| WiFi not connecting   | Enable WPA2 in router settings    |
| Battery not detected  | Calibrate in `battery.cpp`:       |
|                       | `battery.begin(4200, 3.2);`       |
| HTTPS connection fail | Generate new certs:               |
|                       | `tools/generate_certs.sh`         |

---
