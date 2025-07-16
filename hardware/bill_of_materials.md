### SecureVault Pro - Bill of Materials (v2.0)

| Category         | Component                     | Specs/Model                           | Qty | Purpose                          | Est. Cost |
|------------------|-------------------------------|---------------------------------------|-----|----------------------------------|-----------|
| **Core Module**  | ESP32-S3 Dev Board            | ESP32-S3-WROOM-1-N8R8 (16MB Flash)    | 1   | Main processor                   | $8-12     |
| **Display**      | SH1106 OLED                   | 1.3" 128x64 I2C                       | 1   | User interface                   | $4-6      |
| **Input**        | Rotary Encoder                | EC11 + Push Button                    | 1   | Code input/menu navigation       | $1-2      |
| **Locking**      | MG996R Servo Motor            | Metal Gear, 10kg/cm torque            | 1   | Locking mechanism                | $6-8      |
| **Power**        | 18650 Li-ion Battery          | 3.7V 2500mAh                          | 1   | Backup power source              | $4-5      |
|                  | TP4056 Charging Module        | 1A Charge Rate                        | 1   | Battery management               | $1-2      |
|                  | DC-DC Boost Converter         | MT3608 (5V 2A Output)                 | 1   | Stable 5V power                  | $0.50     |
| **Security**     | SW-420 Vibration Sensor       | Sensitivity Adjustable                | 1   | Tamper detection                | $0.80     |
|                  | Reed Switch + Magnets         | Normally Open                         | 1   | Lid opening detection           | $1.50     |
| **Connectivity** | 2.4GHz WiFi Antenna           | External SMA                          | 1   | Enhanced WiFi range             | $2-3      |
| **Misc**         | Prototype PCB                 | 7×9 cm                                | 1   | Main circuit board              | $2        |
|                  | Jumper Wires                  | 20cm Female-Female                    | 10  | Connections                     | $1        |
|                  | Micro USB Cable               | Type B                                | 1   | Power/programming               | $1        |
|                  | 3D Printed Enclosure          | PETG Filament                         | -   | Housing                         | $1.50     |
| **Resistors**    | 10kΩ Resistor                 | 1/4W                                  | 4   | Encoder/Sensor pullups          | $0.10     |
|                  | 1kΩ Resistor                  | 1/4W                                  | 2   | LED current limiting            | $0.05     |
| **Capacitors**   | 100μF Electrolytic            | 16V                                   | 2   | Power stabilization             | $0.20     |
|                  | 0.1μF Ceramic                 | SMD 0805                              | 5   | Noise filtering                 | $0.05     |
| **Transistors**  | 2N2222 NPN Transistor         | TO-92                                 | 1   | Servo power control             | $0.15     |
| **Diodes**       | 1N4007                        | 1A Rectifier                          | 1   | Reverse polarity protection     | $0.10     |
| **LEDs**         | 5mm Red LED                   | Diffused                              | 1   | Status indicator                | $0.05     |

**Total Estimated Cost: $35-45** (excluding 3D printing)

---

### Key Upgrades from Original:
1. **ESP32-S3** → USB-C, 16MB flash, better security
2. **MG996R Servo** → 3× torque vs SG90
3. **SH1106 OLED** → Higher contrast vs SSD1306
4. **Power System** → Battery backup with charging
5. **Security Sensors** → Tamper + lid detection
6. **Professional PCB** → Reliable connections

---

### Recommended Suppliers:
1. **Main Components**: 
   - DigiKey (ESP32-S3, quality sensors)
   - Adafruit (OLED, charger modules)
2. **Economy Parts**:
   - LCSC (Resistors/Capacitors)
   - AliExpress (Enclosures, cables)
3. **3D Printing**:
   - JLCPCB (Metal-infused PETG)
   - Local FabLab (For prototypes)

---

### Critical Notes:
1. **Battery Safety**:
   - Use protected 18650 cells
   - Include temperature monitoring in code
   ```cpp
   // In servo_control.cpp
   if(battery.getBatteryTemp() > 45) disableCharging();
   ```

2. **Security Components**:
   - Calibrate SW-420 sensitivity
   - Position reed switch near lid edge
   - Implement sensor fusion in code:
   ```cpp
   // In security.cpp
   if(vibrationDetected() && lidOpened()) triggerAlarm();
   ```

3. **Production Tips**:
   - Use lead-free solder (RoHS compliant)
   - Apply conformal coating for moisture protection
   - Implement secure bootloader
   ```ini
   ; platformio.ini addition
   board_build.secure = yes
   ```

---

### Optional Enhancements:
| Component        | Benefit                       | Cost  |
|------------------|-------------------------------|-------|
| BME280 Sensor    | Environmental monitoring      | $3    |
| Piezo Buzzer     | Audible alerts                | $0.50 |
| RGB LED          | Status colors                 | $0.30 |
| RTC Module       | Time-based locking            | $1.50 |

This BOM reflects a production-ready version with industrial-grade components. For prototyping, some components can be substituted with cheaper alternatives, but critical security parts (ESP32-S3, MG996R) should not be compromised.
