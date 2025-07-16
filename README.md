# SecureVault Pro - ESP32-Based Security System

## Overview  
SecureVault Pro is an advanced access control system combining physical and digital security mechanisms. Built around the ESP32-S3 microcontroller, it features encrypted wireless management, tamper detection, and battery-backed operation for reliable security enforcement.  

## Key Features  
- **Multi-Factor Authentication**: Rotary encoder PIN entry + encrypted web interface  
- **Industrial-Grade Locking**: MG996R servo with 10kg/cm holding torque  
- **Security Monitoring**: Vibration and lid position sensors with active response  
- **Power Resilient**: 18650 battery backup maintains lock state during outages  
- **Enterprise Encryption**: TLS 1.3 protected management interface  

## Technical Specifications  
- **Microcontroller**: ESP32-S3 (16MB flash, 8MB PSRAM)  
- **Wireless**: 2.4GHz WiFi 4 (802.11n) with external antenna  
- **Power Input**: 5V USB-C or 3.7V Li-ion battery  
- **Display**: SH1106 1.3" OLED (128x64 pixels)  
- **Physical Interface**: EC11 rotary encoder with push button  
- **Security Sensors**: SW-420 vibration detector + reed switch  
- **Operating Temperature**: 0°C to 50°C  

## Installation  
1. Flash firmware using PlatformIO:  
```bash
pio run -t upload
```  

2. Assemble mechanical components following [assembly guide](docs/assembly_guide.md)  

3. Configure WiFi via serial console or web portal:  
```
> wifi_setup home_ssid home_password
```  

## Usage  
**Local Access**:  
- Rotate encoder to select digits  
- Press to confirm each digit  
- 4-digit PIN unlocks servo mechanism  

**Remote Management**:  
```bash
curl -k -H "X-API-Key: YOUR_KEY" https://vault-ip/control \
  -d '{"action":"unlock","pin":"1234"}'
```  

## Documentation  
- [Hardware Schematics](hardware/schematics/schematics.md)  
- [API Reference](docs/api_reference.md)  
- [Security Architecture](docs/security_features.md)  
- [Firmware Development](firmware/README.md)  

## Compliance  
- NIST SP 800-193 (Platform Firmware Resilience)  
- OWASP IoT Top 10 Controls  
- GDPR Article 32 Requirements  

## License  
Released under the MIT LICENSE.

## Support  
For technical inquiries, open an issue in this repository or contact security@securevault.pro. Include device revision number from bottom label in all communications.  

## Contributing  
Submit pull requests to the development branch. All contributions must include signed-off developer certificate of origin. Security vulnerabilities should be reported via encrypted email to security@securevault.pro using PGP key 0x8F3C2A1B.  

## Version History  
- v1.0 (2025-01-15): Initial release  
- v2.0 (2025-07-10): Added battery backup and enhanced tamper detection
