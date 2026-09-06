
/*
More advanced library
Logs: 
  - protocol
  - Value

NEC not working for cheap remotes



*/


#include <IRremote.hpp>

const int RECV_PIN = 7;

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for Serial on boards that need it (e.g. Leonardo)
  Serial.println("Serial Monitor started, listening for a signal");

  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  Serial.print("Ready to receive IR signals on pin ");
  Serial.println(RECV_PIN);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("Protocol: ");
    Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));
    Serial.print("Value: 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    IrReceiver.resume(); // ready for the next signal
  }
}