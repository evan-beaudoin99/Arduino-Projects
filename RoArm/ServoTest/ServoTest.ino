/*
  RoArm-M2 Base Servo Test
  -------------------------
  The RoArm-M2's base joint is a Waveshare/Feetech ST3215 serial bus servo,
  NOT a standard PWM hobby servo. It communicates over a single-wire
  half-duplex UART bus, so it can't be driven with the normal Servo.h
  library — this uses the "SCServo" library instead.

  Library: install "SCServo" via Library Manager, or from
  https://github.com/waveshare/SCServo (Feetech/Waveshare bus servo driver).

  Wiring:
    - Servo bus data line -> a UART TX/RX pair tied together (half-duplex),
      or a board with true half-duplex UART support (e.g. ESP32).
    - Servo bus GND -> board GND
    - Servo power (6-12V per ST3215 spec) from a separate supply, not the
      board's 5V rail. Share GND between the servo supply and the board.
    - Update SERVO_SERIAL below to whichever hardware serial port is wired
      to the bus (Serial1/Serial2/etc. — a board with only one USB Serial,
      like an Uno, can't run this test as-is).

  This sketch sweeps the base joint (ID 1) between two positions and
  prints the servo's reported position back, so you can confirm the
  motor responds and reports feedback correctly.
*/

#include <SCServo.h>

#define SERVO_SERIAL Serial1   // change to match your wiring
#define BASE_SERVO_ID 1        // RoArm-M2 base joint ID

const int CENTER_POS = 2048;   // ST3215 range is 0-4095 (12-bit, 0-360 deg)
const int SWEEP_OFFSET = 300;  // ~26 degrees either side of center
const int MOVE_SPEED = 800;    // 0-4095, higher = faster
const int MOVE_ACCEL = 50;     // 0-255

SMS_STS st;

void moveAndReport(int position) {
  st.WritePosEx(BASE_SERVO_ID, position, MOVE_SPEED, MOVE_ACCEL);
  delay(1500);

  int feedbackPos = st.ReadPos(BASE_SERVO_ID);
  if (feedbackPos != -1) {
    Serial.print("Commanded: ");
    Serial.print(position);
    Serial.print("  Reported: ");
    Serial.println(feedbackPos);
  } else {
    Serial.println("No response from servo - check wiring/ID/power.");
  }
}

void setup() {
  Serial.begin(115200);
  SERVO_SERIAL.begin(1000000);
  st.pSerial = &SERVO_SERIAL;

  Serial.println("RoArm-M2 base servo test starting...");
  moveAndReport(CENTER_POS);
}

void loop() {
  moveAndReport(CENTER_POS + SWEEP_OFFSET);
  moveAndReport(CENTER_POS - SWEEP_OFFSET);
}
