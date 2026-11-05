
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  // USB serial monitor
  Serial.begin(115200);

  // Start Bluetooth Classic
  SerialBT.begin("ESP32_Controller");

  Serial.println("ESP32 Bluetooth controller started.");
  Serial.println("Connect and send 1 or 0.");
}

void loop() {
  // Read a command received over Bluetooth
  if (SerialBT.available()) {
    char command = SerialBT.read();

    // Display the received command
    Serial.print("Received: ");
    Serial.println(command);

    // Forward the command to the Arduino
    // via the USB Serial Monitor for now
  }

  // You can add additional control logic here
}
