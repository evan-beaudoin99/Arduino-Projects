const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

int hue = 0; // 0-359

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    int r, g, b;
    hueToRGB(hue, r, g, b);

    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);

    hue = (hue + 1) % 360;
    delay(15);
}

// Converts a hue (0-359, full saturation/brightness) to 0-255 RGB values.
void hueToRGB(int h, int &r, int &g, int &b) {
    int region = h / 60;
    int remainder = (h % 60) * 255 / 60;

    switch (region) {
        case 0: r = 255; g = remainder; b = 0; break;
        case 1: r = 255 - remainder; g = 255; b = 0; break;
        case 2: r = 0; g = 255; b = remainder; break;
        case 3: r = 0; g = 255 - remainder; b = 255; break;
        case 4: r = remainder; g = 0; b = 255; break;
        default: r = 255; g = 0; b = 255 - remainder; break;
    }
}
