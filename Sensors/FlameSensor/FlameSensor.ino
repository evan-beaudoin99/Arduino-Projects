const int ledPin     = 13;  // on-board Arduino LED
const int digitalPin = 2;   // KY-026 D0
const int analogPin  = A0;  // KY-026 A0

const int FLAME_STATE = LOW; // D0 level when a flame is present (LOW on most boards)

const unsigned long DEBOUNCE_MS = 50; // ignore D0 flicker shorter than this
int lastReading        = HIGH;
int flameState         = HIGH;
unsigned long lastEdge = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(digitalPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // --- Digital output (D0): debounced flame / no-flame switch ---
  int reading = digitalRead(digitalPin);
  if (reading != lastReading) {
    lastEdge = millis();        // input changed: restart the debounce timer
    lastReading = reading;
  }
  if (millis() - lastEdge > DEBOUNCE_MS) {
    flameState = reading;       // stable long enough: accept the new level
  }

  bool flame = (flameState == FLAME_STATE);
  digitalWrite(ledPin, flame ? HIGH : LOW);

  // --- Analog output (A0): raw infrared intensity ---
  // Higher reading = less infrared; the value drops as a flame gets closer.
  int analogVal = analogRead(analogPin);
  Serial.println(analogVal);    // Tools > Serial Plotter to graph it

  delay(100);
}