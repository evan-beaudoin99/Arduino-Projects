// 16x2 LCD + DS1307 real-time clock
//
// Libraries needed (Library Manager):
//   - LiquidCrystal   (usually bundled with the Arduino IDE)
//   - RTClib          by Adafruit
//
// LCD wiring (parallel, same as LCD-16x2-Basic):
//   rs=7 en=8 d4=9 d5=10 d6=11 d7=12, plus V0 contrast pot, R/W->GND, VDD->5V, VSS->GND
//
// DS1307 wiring (I2C):
//   VCC -> 5V, GND -> GND, SDA -> A4 (Uno/Nano) or SDA pin, SCL -> A5 (Uno/Nano) or SCL pin

#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

RTC_DS1307 rtc;

const char *dayNames[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup() {
    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.print("Starting...");

    if (!rtc.begin()) {
        lcd.clear();
        lcd.print("RTC not found");
        Serial.println("Couldn't find RTC");
        while (1) delay(1000);
    }

    if (!rtc.isrunning()) {
        Serial.println("RTC not running, setting to compile time");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    lcd.clear();
}

void loop() {
    DateTime now = rtc.now();

    lcd.setCursor(0, 0);
    printTwoDigits(now.hour());
    lcd.print(":");
    printTwoDigits(now.minute());
    lcd.print(":");
    printTwoDigits(now.second());
    lcd.print("  ");
    lcd.print(dayNames[now.dayOfTheWeek()]);

    lcd.setCursor(0, 1);
    printTwoDigits(now.month());
    lcd.print("/");
    printTwoDigits(now.day());
    lcd.print("/");
    lcd.print(now.year());
    lcd.print("  ");

    delay(500);
}

void printTwoDigits(int value) {
    if (value < 10) {
        lcd.print("0");
    }
    lcd.print(value);
}
