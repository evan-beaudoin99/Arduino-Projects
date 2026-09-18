// Reads the digital state of a pushbutton and prints it to Serial.
//
// Wiring (using the internal pull-up, no external resistor needed):
//   Button pin 1 -> Arduino digital pin (BUTTON_PIN)
//   Button pin 2 -> GND
//
// With INPUT_PULLUP the pin reads HIGH when the button is NOT pressed
// and LOW when it IS pressed (the button connects the pin to GND).

const int BUTTON_PIN = 2;
const int LED_PIN = 13; // onboard LED, mirrors the button state

int lastButtonState = HIGH;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    int buttonState = digitalRead(BUTTON_PIN);
    bool isPressed = (buttonState == LOW);

    digitalWrite(LED_PIN, isPressed ? HIGH : LOW);

    if (buttonState != lastButtonState) {
        Serial.println(isPressed ? "Button PRESSED" : "Button released");
        lastButtonState = buttonState;
    }
}
