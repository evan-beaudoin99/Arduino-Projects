#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "CMU";
const char* password = "6138841982";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  getReq();
}

void loop() {
  // Optional: check connection status periodically
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.begin(ssid, password);
    delay(5000);
  }
}


void getReq() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; 
    http.begin("https://gateway.timeapi.world/timezone/America/Chicago");
    int httpCode = http.GET();
    if (httpCode > 0) Serial.println(http.getString());
    http.end();
  }
}