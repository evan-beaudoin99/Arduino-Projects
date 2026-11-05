
#include <Servo.h>

Servo servo;

void setup() {

    Serial.begin(9600);

    servo.attach(9);
}

void loop() {

    //int pos_x = analogRead(A5);

   // int angle = map(pos_x, 0, 1023, 0, 180);

    servo.write(0);
    delay(1000);
    servo.write(90);
    delay(1000);

    // Serial.println(pos_x);
}