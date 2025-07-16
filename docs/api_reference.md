# SecureVault Pro - API Reference  
**Version 2.0 | Authentication Required**  

---

## Base URL  
`https://<DEVICE_IP>` (Default: `https://192.168.4.1`)  
*Note: HTTPS enforced with TLS 1.3 - Ignore browser warnings for self-signed cert*  

---

## Authentication  
All endpoints require API key in request header:  
```http
X-API-Key: SECRET-KEY-1234
```  
*Change default key in `security.h` before deployment*  

---

## Endpoints  

### 1. Get System Status  
```http
GET /status  
```  
**Response**  
```json
{
  "locked": true,
  "battery_voltage": 3.82,
  "failed_attempts": 2,
  "tamper_detected": false,
  "firmware_version": "2.0.1"
}
```  

### 2. Set New PIN Code  
```http
POST /set_pin  
Content-Type: application/x-www-form-urlencoded  

pin=5678
```  
**Parameters**  
- `pin` (required): 4-digit code (no spaces)  

**Responses**  
- `200 OK`: `{"status": "PIN updated"}`  
- `400 Bad Request`: `{"error": "Invalid PIN format"}`  
- `401 Unauthorized`: Missing/invalid API key  

### 3. Lock/Unlock Vault  
```http
POST /control  
Content-Type: application/json  

{
  "action": "unlock",
  "pin": "1234"
}
```  
**Parameters**  
- `action` (required): `lock` or `unlock`  
- `pin` (required for unlock): Current 4-digit code  

**Responses**  
- `200 OK`: `{"status": "Vault unlocked"}`  
- `403 Forbidden`: `{"error": "Invalid PIN"}`  
- `423 Locked`: `{"error": "Locked out until 15:32:45"}`  

### 4. OTA Firmware Update  
```http
POST /update  
Content-Type: multipart/form-data  
X-API-Key: SECRET-KEY-1234  

[Binary firmware file]
```  
**Parameters**  
- Firmware binary in `firmware` field  

**Responses**  
- `200 OK`: Update successful (auto-reboot)  
- `400 Bad Request`: Invalid firmware format  
- `500 Internal Error`: Flash write failed  

### 5. Emergency Lock  
```http
POST /lockdown  
```  
**Response**  
```json
{"status": "Permanently locked"}
```  
*Irreversible without physical reset*  

---

## Error Codes  
| Status | Meaning                  | Resolution              |
|--------|--------------------------|-------------------------|
| 401    | Missing/invalid API key  | Check X-API-Key header |
| 423    | Temporary lockout        | Wait timeout period     |
| 429    | Too many requests        | Slow down requests      |
| 500    | Internal server error    | Check device logs       |
| 503    | Low battery              | Charge device           |

---

## Example Requests  

### cURL - Set New PIN  
```bash
curl -k -X POST https://192.168.4.1/set_pin \
  -H "X-API-Key: SECRET-KEY-1234" \
  -d "pin=9876"
```

### Python - Check Status  
```python
import requests

response = requests.get(
    "https://192.168.4.1/status",
    headers={"X-API-Key": "SECRET-KEY-1234"},
    verify=False  # Bypass self-signed cert
)
print(response.json())
```

### JavaScript - Unlock  
```javascript
fetch('https://192.168.4.1/control', {
  method: 'POST',
  headers: {
    'X-API-Key': 'SECRET-KEY-1234',
    'Content-Type': 'application/json'
  },
  body: JSON.stringify({
    action: 'unlock',
    pin: '1234'
  })
})
.then(response => response.json())
.then(data => console.log(data));
```

---

## Security Notes  
1. Always use HTTPS  
2. Rotate API keys monthly  
3. Limit request rate to 5 requests/minute  
4. Firmware updates require SHA-256 signed binaries  
5. After 10 failed API attempts, device enters 24-hour lockdown  

---

## Troubleshooting  
**SSL Errors**  
```bash
# cURL bypass
curl --insecure

# Python workaround
import urllib3
urllib3.disable_warnings()
```

**Connection Refused**  
1. Confirm device is powered  
2. Verify IP address hasn't changed  
3. Check for firewall blocking port 443  

**Version History**  
| Endpoint       | v1.0 (Deprecated) | v2.0 Current |
|----------------|-------------------|--------------|
| Set PIN        | /set?pin=1 2 3 4  | /set_pin     |
| Status         | Not available     | /status      |
| Authentication | URL parameter     | Header       |
