#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Shield-specific driver library

MCUFRIEND_kbv tft;

// Assign human-readable names to common 16-bit color values
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup() {
    Serial.begin(9600);
    
    // Read the driver ID chip inside the shield
    uint16_t ID = tft.readID();
    Serial.print("TFT ID found: 0x");
    Serial.println(ID, HEX);
    
    // If the ID reads 0xD3D3 (common override issue), manually force the driver ID
    if (ID == 0xD3D3) ID = 0x9486; // Force ILI9486
    
    tft.begin(ID);
    
    // Set screen orientation: 0 & 2 are Portrait, 1 & 3 are Landscape
    tft.setRotation(1); 
    
    // Fill the background color
    tft.fillScreen(BLACK);
    
    // Draw visual accents
    tft.drawRect(10, 10, tft.width() - 20, tft.height() - 20, YELLOW);
    tft.fillRect(30, 30, 80, 50, BLUE);

    // Display Text Message
    tft.setCursor(130, 45);          // Coordinate layout: X, Y
    tft.setTextColor(WHITE);         // Text color
    tft.setTextSize(3);              // Scale multiplier size (1-8)
    tft.print("Mega 2560 OK!");
}

void loop() {
    // Add dynamic UI updates or cycles here
    tft.setCursor(30, 150);
    tft.setTextColor(GREEN, BLACK); // Green text with a Black text-background block
    tft.setTextSize(2);
    tft.print("Uptime (ms): ");
    tft.print(millis());
    delay(100);
}