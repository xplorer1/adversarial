// ----- ESP32 #1: RFID Reader (client) -----
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>

// RFID pins
#define RST_PIN  4
#define SS_PIN   5
MFRC522 reader(SS_PIN, RST_PIN);

// Wi-Fi and Node server config.
//Note that the ESP32 must be connected to the same network as the Node.js server.
//The ESP32 can only connect to a 2.4GHz network, not a 5GHz network.
const char* ssid     = "<Your WIFI SSID>";
const char* password = "<Your WIFI Password>";
const char* nodeHost = "<Your local server/IP Adress of IPV4 network."; // Node.js server IP

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n[SETUP] Starting up...");

  // Initialize SPI bus for RFID
  SPI.begin(18, 19, 23, SS_PIN);
  reader.PCD_Init();
  Serial.println("[SETUP] RFID reader initialized");

  // Connect to Wi-Fi
  Serial.printf("[SETUP] Connecting to WiFi SSID: %s\n", ssid);
  WiFi.begin(ssid, password);
  unsigned long startMillis = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startMillis < 30000) {
    Serial.print('.');
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[SETUP] WiFi connected");
    Serial.print("[SETUP] IP address: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[ERROR] WiFi connection failed or timed out");
  }
}

void loop() {
  if (!reader.PICC_IsNewCardPresent()) {
    delay(500);
    return;
  }
  if (!reader.PICC_ReadCardSerial()) {
    Serial.println("[LOOP] Card present but could not read UID");
    delay(500);
    return;
  }
  Serial.println("[LOOP] Card detected, reading UID...");

  // Build UID string
  String uid;
  for (byte i = 0; i < reader.uid.size; i++) {
    if (reader.uid.uidByte[i] < 0x10) uid += '0';
    uid += String(reader.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  Serial.print("[LOOP] UID read: "); Serial.println(uid);

  // Send to Node server for verification
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(nodeHost) + "/api/user/locker/unlock";
    String payload = String("{\"mode\":\"rfid\",\"rfid_code\":\"") + uid + "\"}";

    Serial.print("[HTTP] POST URL: "); Serial.println(url);
    Serial.print("[HTTP] Payload: "); Serial.println(payload);

    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(payload);
    Serial.printf("[HTTP] POST returned: %d\n", code);
    if (code > 0) {
      String response = http.getString();
      Serial.print("[HTTP] Response: "); Serial.println(response);
    } else {
      Serial.printf("[ERROR] HTTP POST failed, error: %s\n", http.errorToString(code).c_str());
    }
    http.end();
  } else {
    Serial.println("[ERROR] WiFi not connected, cannot send HTTP request");
  }

  // Cleanup RFID state
  reader.PICC_HaltA();
  reader.PCD_StopCrypto1();

  delay(2000);
}