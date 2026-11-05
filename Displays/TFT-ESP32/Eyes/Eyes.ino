
#include <TFT_eSPI.h>
#include <math.h>

TFT_eSPI tft = TFT_eSPI();

// Eye settings
const int eyeWidth = 86;
const int eyeHeight = 100;

const int leftEyeX = 100;
const int rightEyeX = 220;
const int eyeY = 120;

const uint16_t backgroundColor = TFT_BLACK;
const uint16_t eyeColor = TFT_WHITE;
const uint16_t irisColor = TFT_CYAN;
const uint16_t pupilColor = TFT_BLUE;

unsigned long lastBlink = 0;
const unsigned long blinkInterval = 4000;

// Draw one eye
void drawEye(int x, int y, int pupilOffset, bool blinking) {

  // Clear previous eye
  tft.fillRect(
    x - eyeWidth / 2 - 2,
    y - eyeHeight / 2 - 2,
    eyeWidth + 4,
    eyeHeight + 4,
    backgroundColor
  );

  // Blinking animation
  if (blinking) {
    tft.fillRoundRect(
      x - eyeWidth / 2,
      y - 3,
      eyeWidth,
      6,
      3,
      irisColor
    );
    return;
  }

  // White eye
  tft.fillRoundRect(
    x - eyeWidth / 2,
    y - eyeHeight / 2,
    eyeWidth,
    eyeHeight,
    35,
    eyeColor
  );

  // Iris
  int pupilX = x + pupilOffset;

  tft.fillCircle(pupilX, y, 29, irisColor);

  // Pupil
  tft.fillCircle(pupilX, y, 19, pupilColor);

  // Eye highlight
  tft.fillCircle(pupilX - 8, y - 9, 7, TFT_WHITE);
}

// Draw both eyes
void drawFace(int pupilOffset, bool blinking) {

  drawEye(leftEyeX, eyeY, pupilOffset, blinking);
  drawEye(rightEyeX, eyeY, pupilOffset, blinking);
}

void setup() {

  tft.init();

  // Landscape orientation
  tft.setRotation(1);

  tft.fillScreen(backgroundColor);

  drawFace(0, false);
}

void loop() {

  unsigned long currentTime = millis();

  // Blink every four seconds
  bool blinking = (currentTime - lastBlink > blinkInterval);

  if (blinking) {
    drawFace(0, true);
    delay(150);

    lastBlink = millis();
    blinking = false;
  }

  // Move pupils smoothly
  int movement = (int)(12 * sin(currentTime / 700.0));

  drawFace(movement, blinking);

  delay(30);
}
