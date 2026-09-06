


#include <IRremote.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup(){
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  Serial.println("Serial Monitor started, listening for a signal");
  irrecv.enableIRIn();
  // irrecv.blink13(true);
}

void loop(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
  }
}
