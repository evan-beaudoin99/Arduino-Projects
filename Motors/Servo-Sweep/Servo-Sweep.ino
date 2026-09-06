#include <Servo.h>

const int servoPin = 9;
Servo myServo;

void setup() {
  myServo.attach(servoPin);
}

void loop() {
  // sweep 0 -> 180
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle);
    delay(15);
  }

  // sweep 180 -> 0
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(15);
  }
}
