const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int numLeds = 12;

void setAll(bool state) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], state);
  }
}

// one LED runs left to right, then back
void bounce(int stepDelay) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(stepDelay);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = numLeds - 2; i > 0; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(stepDelay);
    digitalWrite(ledPins[i], LOW);
  }
}

// a 3-LED train with a fading tail chases along the strip
void train(int stepDelay) {
  for (int i = 0; i < numLeds + 3; i++) {
    setAll(LOW);
    for (int t = 0; t < 3; t++) {
      int p = i - t;
      if (p >= 0 && p < numLeds) digitalWrite(ledPins[p], HIGH);
    }
    delay(stepDelay);
  }
}

// two LEDs start at the ends and meet in the middle, then split out
void collide(int stepDelay) {
  for (int i = 0; i < numLeds / 2; i++) {
    setAll(LOW);
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[numLeds - 1 - i], HIGH);
    delay(stepDelay);
  }
  for (int i = numLeds / 2 - 1; i >= 0; i--) {
    setAll(LOW);
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[numLeds - 1 - i], HIGH);
    delay(stepDelay);
  }
}

// every other LED, swapping sides
void alternate(int times, int stepDelay) {
  for (int n = 0; n < times; n++) {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], (i + n) % 2 == 0);
    }
    delay(stepDelay);
  }
}

// random sparkle
void sparkle(int times, int stepDelay) {
  for (int n = 0; n < times; n++) {
    digitalWrite(ledPins[random(numLeds)], HIGH);
    digitalWrite(ledPins[random(numLeds)], LOW);
    delay(stepDelay);
  }
  setAll(LOW);
}

void flash(int times, int stepDelay) {
  for (int n = 0; n < times; n++) {
    setAll(HIGH);
    delay(stepDelay);
    setAll(LOW);
    delay(stepDelay);
  }
}

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  randomSeed(analogRead(A0)); // floating pin gives a different show each power-up
}

void loop() {
  bounce(60);
  train(70);
  collide(120);
  alternate(10, 200);
  sparkle(60, 60);
  flash(4, 120);
  train(30); // fast finish
}
