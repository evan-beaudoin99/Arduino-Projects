#include <SCServo.h>

SMS_STS servo;

#define SERVO_RX 18
#define SERVO_TX 19

#define BASE_SERVO_ID 11

void setup() {
  Serial.begin(115200);

  Serial1.begin(1000000, SERIAL_8N1, SERVO_RX, SERVO_TX);

  servo.pSerial = &Serial1;

  delay(1000);

  Serial.println("RoArm-M2 Base Servo Test");
  Serial.println("Starting movement test...");
}

void loop() {

  Serial.println("Moving to position 2000...");
  servo.WritePosEx(BASE_SERVO_ID, 2000, 500, 100);
  delay(2000);

  Serial.println("Moving to position 2200...");
  servo.WritePosEx(BASE_SERVO_ID, 2200, 500, 100);
  delay(2000);

  Serial.println("Moving to position 2000...");
  servo.WritePosEx(BASE_SERVO_ID, 2000, 500, 100);
  delay(2000);
}