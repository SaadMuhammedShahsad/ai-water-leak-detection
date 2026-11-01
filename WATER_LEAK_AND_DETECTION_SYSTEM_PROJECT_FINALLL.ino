// TELEGRAM WATER LEAK ALERT – ESP8266 (NodeMCU)
// Pins: SENSOR = D2, BUZZER = D5, LED = D6
// Uses HTTPS via WiFiClientSecure (built into ESP8266 core)

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

// ---------- FILL THESE ----------
const char* WIFI_SSID = "Saad Shahzad";        // replace if different
const char* WIFI_PASS = "saad1234";           // replace with your WiFi password

const String BOT_TOKEN = "8509106642:AAE0-TiGKeMx0kqiUgsIHwB7o64knZCxCjc";    // e.g. "8509...CxCjc"
const String CHAT_ID   = "1413857383";      // e.g. "1413857383"
// ---------- END FILL ----------

const int SENSOR_PIN = D2;   // water sensor input
const int BUZZER_PIN = D5;   // buzzer output
const int LED_PIN    = D6;   // LED output

bool alertSent = false;
unsigned long lastAlertMs = 0;
const unsigned long alertCooldown = 30UL * 1000UL; // 30 seconds cooldown

// Helper: URL-encode text for safe GET request
String urlEncode(const String &str) {
  String encoded = "";
  char buf[5];
  for (unsigned int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if ( ('0' <= c && c <= '9') ||
         ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         c == '-' || c == '_' || c == '.' || c == '~' ) {
      encoded += c;
    } else if (c == ' ') {
      encoded += '+';
    } else {
      snprintf(buf, sizeof(buf), "%%%02X", (unsigned char)c);
      encoded += buf;
    }
  }
  return encoded;
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int sensorValue = digitalRead(SENSOR_PIN);
  Serial.print("Sensor: ");
  Serial.println(sensorValue); // 0 or 1

  if (sensorValue == HIGH) { // adjust if your sensor is active LOW
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    unsigned long now = millis();
    if (!alertSent || (now - lastAlertMs) > alertCooldown) {
      sendTelegramAlert("🚨 Water Leak Detected! Check immediately.");
      alertSent = true;
      lastAlertMs = now;
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    alertSent = false; // ready for next event
  }

  delay(300);
}

void sendTelegramAlert(const String &message) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected — cannot send alert");
    return;
  }

  Serial.println("Sending Telegram alert...");

  WiFiClientSecure client;
  client.setInsecure(); // avoids certificate validation issues (ok for hobby projects)

  HTTPClient https;
  String url = "https://api.telegram.org/bot" + BOT_TOKEN +
               "/sendMessage?chat_id=" + CHAT_ID +
               "&text=" + urlEncode(message) +
               "&parse_mode=Markdown";

  if (https.begin(client, url)) {
    int httpCode = https.GET();
    Serial.print("Telegram HTTP code: ");
    Serial.println(httpCode); // 200 = OK
    if (httpCode == 200) {
      Serial.println("Alert sent successfully.");
    } else {
      // Read response body for debug
      String payload = https.getString();
      Serial.print("Response body: ");
      Serial.println(payload);
    }
    https.end();
  } else {
    Serial.println("HTTPS begin failed");
  }
}






