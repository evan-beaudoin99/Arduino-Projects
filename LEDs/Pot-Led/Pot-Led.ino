#define POT_PIN A5
const int maxLEDS = 8;

// Digital pins driving each LED, in order.
const int leds[maxLEDS] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
    Serial.begin(9600);
    pinMode(POT_PIN, INPUT);

    for (int i = 0; i < maxLEDS; i++) {
        pinMode(leds[i], OUTPUT);
    }
}

void loop() {
    int reading = analogRead(POT_PIN);              // 0-1023
    int litCount = map(reading, 0, 1023, 0, maxLEDS); // 0-8

    for (int i = 0; i < maxLEDS; i++) {
        digitalWrite(leds[i], i < litCount ? HIGH : LOW);
    }

    Serial.println(reading);
    delay(50);
}
