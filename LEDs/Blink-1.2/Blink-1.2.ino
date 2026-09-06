
// blink with function


int ledPin = 13; // Pin connected to the LED

void setup() {
  pinMode(ledPin, OUTPUT); // Set the digital pin as an output
}

void loop() {
  blink(1000); // Call the function and blink for 1 second (1000ms)
}

// Custom function to handle the blinking logic
void blink(int duration) {
  digitalWrite(ledPin, HIGH); // Turn the LED on
  delay(duration);            // Wait
  digitalWrite(ledPin, LOW);  // Turn the LED off
  delay(duration);            // Wait
}
