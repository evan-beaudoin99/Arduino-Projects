#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const char* ssid = "CMU";
const char* password = "6138841982";

const char* serverUrl = "https://live-stock-market.p.rapidapi.com/v1/index/chart";

void setup() {
  Serial.begin(115200);
  delay(1000);

  JsonDocument doc;

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

}

void loop() {
  // Optional: check connection status periodically
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClientSecure client; 

    client.setInsecure();

    HTTPClient http;

    http.begin(client, serverUrl); // Establish the HTTP client with the secure WiFI connection and target URL 

    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-rapidapi-host", "live-stock-market.p.rapidapi.com");
    http.addHeader("x-rapidapi-key", "59a3d41ce7msh2876af872da5341p1cbc08jsn677e7f2a2e6e");

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code");
      Serial.println(httpResponseCode);

      // Handle successful responses
      if (httpResponseCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Received Payload:");
        Serial.println(payload);
      }
    } else {
      Serial.print("Connection Error: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    // Terminate connection to free up RAM heap memory
    http.end();

    delay(30000);

  } else {
    Serial.println("Error: Lost WiFi connection.");
    WiFi.begin(ssid, password);
    Serial.println("Connecting...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // Poll the API once every 5 minutes to stay within rate limits
  // delay(300000); 
}

