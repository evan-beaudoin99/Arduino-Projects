#include <LiquidCrystal.h>

// Wiring (standard HD44780 16x2, 4-bit mode):
// RS -> 12, E -> 11, D4 -> 5, D5 -> 4, D6 -> 3, D7 -> 2
// Also wire: VSS->GND, VDD->5V, V0->potentiometer wiper (contrast),
// RW->GND, A->5V (backlight +), K->GND (backlight -)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    lcd.begin(16, 2);
    lcd.print("Hello, ABA STEM!");
}

void loop() {
    lcd.setCursor(0, 1);
    lcd.print("Uptime: ");
    lcd.print(millis() / 1000);
    lcd.print("s   "); // trailing spaces clear leftover digits
    delay(200);
}
