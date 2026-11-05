void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 IS ALIVE");
}

void loop() {
  Serial.println("Running...");
  delay(1000);
}