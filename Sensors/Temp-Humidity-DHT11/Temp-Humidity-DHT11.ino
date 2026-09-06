#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000); // DHT11 needs ~2s between reads

  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();

  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Failed to read from DHT11 sensor");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.print(" C  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}
