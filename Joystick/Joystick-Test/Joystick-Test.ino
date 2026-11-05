void setup() {

    Serial.begin(9600);


}

void loop() {

    int pos_x = analogRead(A5);

    Serial.println(pos_x);
}