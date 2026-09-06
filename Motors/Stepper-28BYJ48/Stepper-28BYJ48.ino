#include <Stepper.h>

const int stepsPerRevolution = 2048; // 28BYJ-48 with internal gearing

// ULN2003 driver board IN1, IN2, IN3, IN4
// Wired in this order (not 1,2,3,4) for correct step sequencing
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(10); // RPM
}

void loop() {
  // Serial.println("Clockwise");
  // myStepper.step(stepsPerRevolution);
  // delay(500);

  Serial.println("Counter-clockwise");
  myStepper.step(2048);
  delay(500);
}
