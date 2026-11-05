#define POT_PIN A5
const int GREEN_PIN = 12;
const int YELLOW_PIN = 13;
const int RED_PIN = 11;

void setup() {
    Serial.begin(9600);
    pinMode(POT_PIN, INPUT);

    pinMode(GREEN_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
}

void loop() {
    int reading = analogRead(POT_PIN);         // 0-1023
    int angle = map(reading, 0, 1023, 0, 180); // 0-180

    // Each third of the range lights one LED: green, yellow, then red.
    digitalWrite(GREEN_PIN, angle < 60 ? HIGH : LOW);
    digitalWrite(YELLOW_PIN, (angle >= 60 && angle < 120) ? HIGH : LOW);
    digitalWrite(RED_PIN, angle >= 120 ? HIGH : LOW);

    Serial.println(angle);
    delay(50);
}
