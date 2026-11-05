/*
  ABA STEM - Mini Robot Arm (joystick control)
  ---------------------------------------------
  Hardware
    Arduino Uno
    28BYJ-48 stepper + ULN2003 driver board  -> base rotation
    3x SG90 servo                             -> shoulder, elbow, gripper
    2x KY-023 joystick                        -> controls
    DIY 12V -> 5V supply powers the servos and the ULN2003 board.
    IMPORTANT: the supply GND must be connected to the Arduino GND.

  Controls
    Left stick  left/right : turn the base       up/down : shoulder
    Right stick up/down    : elbow               left/right : gripper open/close
    Press LEFT stick       : return to the home pose
    Press RIGHT stick      : slow (precision) mode on/off  (built-in LED on = slow)

  Assembly helper
    Hold the RIGHT stick pressed while switching on: every servo goes to 90 degrees
    and stays there (built-in LED blinks). Attach the horns/links in that position:
      shoulder 90 = upper arm pointing straight up
      elbow    90 = forearm in line with the upper arm
      gripper  90 = jaws closed
    Also switch on with the turntable arrow pointing forward - that is base "zero".

  Only the built-in Servo library is needed (no extra libraries).
*/
#include <Servo.h>

// ===================== PINS =====================
const uint8_t JL_X = A0, JL_Y = A1, JL_SW = 2;     // left joystick  (VRx, VRy, SW)
const uint8_t JR_X = A2, JR_Y = A3, JR_SW = 4;     // right joystick (VRx, VRy, SW)
const uint8_t PIN_SHOULDER = 9, PIN_ELBOW = 10, PIN_GRIP = 11;
const uint8_t STEP_PIN[4] = {5, 6, 7, 8};          // ULN2003 IN1, IN2, IN3, IN4

// ===================== SETTINGS =====================
const int   DEADZONE     = 70;     // ignore small stick movements (raw counts, 0-512)
const float SERVO_SPEED  = 90.0;   // servo speed in degrees per second at full stick
const float SLOW_FACTOR  = 0.3;    // speed multiplier in slow mode

// Servo limits in degrees (see "Assembly helper" above for what 90 means)
const float SH_MIN = 10,  SH_MAX = 170, SH_HOME = 90;   // shoulder: keeps the arm 10 deg above horizontal
const float EL_MIN = 0,   EL_MAX = 180, EL_HOME = 90;   // elbow
const float GR_CLOSED = 90, GR_OPEN = 135, GR_HOME = 120; // gripper: if it opens the wrong way use 90 / 45

// Flip one of these to true if a joint moves opposite to the stick
const bool INV_BASE = false, INV_SHOULDER = false, INV_ELBOW = false, INV_GRIP = false;

// Base stepper (28BYJ-48 in half-step mode: about 4096 steps per turn)
const long  STEPS_PER_REV  = 4096;
const float BASE_LIMIT_DEG = 80.0;  // the cable slot allows about +/-80 degrees
const float BASE_MAX_SPS   = 700.0; // steps per second at full stick
const float BASE_MIN_SPS   = 80.0;  // steps per second just outside the deadzone

// ===================== STATE =====================
Servo shoulder, elbow, grip;
float shPos = SH_HOME, elPos = EL_HOME, grPos = GR_HOME;
int   cLX, cLY, cRX, cRY;                 // stick centres, measured at start-up
bool  slowMode = false, homing = false;

const uint8_t HALF_STEP[8][4] = {
  {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
  {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}
};
long basePos = 0;                         // steps away from the power-on position
int  phase = 0;
bool coilsOn = false;
unsigned long lastStepUs = 0, lastServoMs = 0, lastPrintMs = 0;

bool lastL = false, lastR = false;
unsigned long tL = 0, tR = 0;

// ===================== HELPERS =====================
int averageRead(uint8_t pin) {
  long sum = 0;
  for (int i = 0; i < 32; i++) { sum += analogRead(pin); delay(2); }
  return sum / 32;
}

// Stick position as -1.0 .. +1.0 with a deadzone around the measured centre
float axis(uint8_t pin, int centre, bool invert) {
  int v = analogRead(pin) - centre;
  if (abs(v) < DEADZONE) return 0.0;
  float span = (v > 0) ? (1023 - centre - DEADZONE) : (centre - DEADZONE);
  float out = (v > 0 ? v - DEADZONE : v + DEADZONE) / span;
  out = constrain(out, -1.0, 1.0);
  return invert ? -out : out;
}

// true once per press (debounced)
bool pressed(uint8_t pin, bool &last, unsigned long &t) {
  bool now = digitalRead(pin) == LOW;
  if (now != last && millis() - t > 40) {
    t = millis();
    last = now;
    return now;
  }
  return false;
}

void stepOnce(int dir) {
  phase = (phase + dir + 8) % 8;
  for (int i = 0; i < 4; i++) digitalWrite(STEP_PIN[i], HALF_STEP[phase][i]);
  basePos += dir;
  coilsOn = true;
}

void coilsOff() {                          // saves ~0.2 A when the base is not moving
  for (int i = 0; i < 4; i++) digitalWrite(STEP_PIN[i], LOW);
  coilsOn = false;
}

void moveToward(float &pos, float target, float maxStep) {
  if (fabs(target - pos) <= maxStep) pos = target;
  else pos += (target > pos) ? maxStep : -maxStep;
}

void writeServo(Servo &s, float deg) {     // microseconds give smoother motion than whole degrees
  s.writeMicroseconds(500 + (int)(deg * (1900.0 / 180.0)));
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(9600);
  pinMode(JL_SW, INPUT_PULLUP);
  pinMode(JR_SW, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(STEP_PIN[i], OUTPUT);
  coilsOff();

  // Don't touch the sticks for the first half second: their centres are measured now
  cLX = averageRead(JL_X);  cLY = averageRead(JL_Y);
  cRX = averageRead(JR_X);  cRY = averageRead(JR_Y);

  shoulder.attach(PIN_SHOULDER, 500, 2400);
  elbow.attach(PIN_ELBOW, 500, 2400);
  grip.attach(PIN_GRIP, 500, 2400);

  if (digitalRead(JR_SW) == LOW) {          // assembly helper
    writeServo(shoulder, 90); writeServo(elbow, 90); writeServo(grip, 90);
    Serial.println(F("ASSEMBLY MODE: all servos at 90 degrees. Attach the horns now."));
    while (true) digitalWrite(LED_BUILTIN, (millis() / 250) % 2);
  }

  writeServo(shoulder, shPos); writeServo(elbow, elPos); writeServo(grip, grPos);
  Serial.println(F("Robot arm ready. Left press = home, right press = slow mode."));
}

// ===================== LOOP =====================
void loop() {
  // ---- buttons
  if (pressed(JL_SW, lastL, tL)) homing = true;
  if (pressed(JR_SW, lastR, tR)) { slowMode = !slowMode; digitalWrite(LED_BUILTIN, slowMode); }
  float speedMul = slowMode ? SLOW_FACTOR : 1.0;

  // ---- sticks
  float bx = axis(JL_X, cLX, INV_BASE);
  float sy = axis(JL_Y, cLY, INV_SHOULDER);
  float ey = axis(JR_Y, cRY, INV_ELBOW);
  float gx = axis(JR_X, cRX, INV_GRIP);
  if (bx != 0 || sy != 0 || ey != 0 || gx != 0) homing = false;   // any stick cancels "home"

  // ---- base stepper (non-blocking)
  long limit = (long)(BASE_LIMIT_DEG / 360.0 * STEPS_PER_REV);
  int dir = 0;
  float sps = 0;
  if (homing) {
    if (basePos != 0) { dir = (basePos > 0) ? -1 : 1; sps = BASE_MAX_SPS * 0.6 * speedMul; }
  } else if (bx != 0) {
    dir = (bx > 0) ? 1 : -1;
    sps = (BASE_MIN_SPS + (BASE_MAX_SPS - BASE_MIN_SPS) * fabs(bx)) * speedMul;
  }
  bool allowed = (dir > 0 && basePos < limit) || (dir < 0 && basePos > -limit);
  if (dir != 0 && allowed) {
    unsigned long interval = (unsigned long)(1000000.0 / sps);
    if (micros() - lastStepUs >= interval) { lastStepUs = micros(); stepOnce(dir); }
  } else if (coilsOn && micros() - lastStepUs > 50000UL) {
    coilsOff();
  }

  // ---- servos, updated every 20 ms
  unsigned long now = millis();
  if (now - lastServoMs >= 20) {
    float dt = (now - lastServoMs) / 1000.0;
    if (dt > 0.1) dt = 0.1;
    lastServoMs = now;
    float stepDeg = SERVO_SPEED * speedMul * dt;
    float gMin = min(GR_CLOSED, GR_OPEN), gMax = max(GR_CLOSED, GR_OPEN);
    float openDir = (GR_OPEN > GR_CLOSED) ? 1.0 : -1.0;

    if (homing) {
      moveToward(shPos, SH_HOME, stepDeg);
      moveToward(elPos, EL_HOME, stepDeg);
      moveToward(grPos, GR_HOME, stepDeg);
      if (shPos == SH_HOME && elPos == EL_HOME && grPos == GR_HOME && basePos == 0) homing = false;
    } else {
      shPos = constrain(shPos + sy * stepDeg, SH_MIN, SH_MAX);
      elPos = constrain(elPos + ey * stepDeg, EL_MIN, EL_MAX);
      grPos = constrain(grPos + gx * openDir * stepDeg, gMin, gMax);
    }
    writeServo(shoulder, shPos);
    writeServo(elbow, elPos);
    writeServo(grip, grPos);
  }

  // ---- status on the Serial Monitor (9600 baud), handy for tuning the limits
  if (now - lastPrintMs >= 500) {
    lastPrintMs = now;
    Serial.print(F("base "));      Serial.print(basePos * 360.0 / STEPS_PER_REV, 1);
    Serial.print(F(" deg | shoulder ")); Serial.print(shPos, 0);
    Serial.print(F(" | elbow "));  Serial.print(elPos, 0);
    Serial.print(F(" | gripper ")); Serial.print(grPos, 0);
    Serial.println(slowMode ? F(" | SLOW") : F(""));
  }
}
