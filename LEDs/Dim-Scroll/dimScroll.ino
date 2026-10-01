int ledPins[8] = {3, 5, 6, 9, 10, 11, 12, 13};

void setup() {
    Serial.begin(9600);

    for (int led = 0; led < 8; led++) {
        pinMode(ledPins[led], OUTPUT);
    }
}

void loop() {
    // 8 LEDs ANALOG VALUE (0-->255)  8 x 255 = 2040
    for (int led = 0; led < 8; led++) {
        for (int value = 0; value < 255; value++) {
            analogWrite(ledPins[led], value);
            Serial.print("LED ");
            Serial.print(led);
            Serial.print(" has ");
            Serial.println(value);
        }
        analogWrite(ledPins[led], 0);
    }
}
