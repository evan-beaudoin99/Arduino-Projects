
#include <iostream>

using namespace std;

int leds[8] = {0};


int main() {

    // 8 LEDs ANALOG VALUE (0-->255)  8 x 255 = 2040
    for (int led = 0; led < 8; led++) {
        for (int value = 0; value < 255; value++) {
            leds[led] = value;
            cout << "LED " << led << " has " << value << endl;
        }
    }

    return 0;
}
