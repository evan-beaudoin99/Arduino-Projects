// Laser trip wire
// LDR wiring: 5V -> LDR -> A0 -> 10k resistor -> GND
// LEDs: pin -> 220 ohm resistor -> LED -> GND
const int ldrPin = A0;
const int redPin = 12;
const int greenPin = 11;

// Below this reading the beam is considered broken (0-1023).
// Tune it: note the raw value with the laser on and off, pick a value between.
const int threshold = 300;

void setup()
{
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  Serial.println("Starting trip wire...");
}

void loop()
{
  int raw = analogRead(ldrPin);
  bool broken = raw > threshold;

  digitalWrite(redPin, broken ? HIGH : LOW);
  digitalWrite(greenPin, broken ? LOW : HIGH);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.println(broken ? "  TRIPPED" : "  Intact");

  delay(50);
}
