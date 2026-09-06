#include <Adafruit_GFX.h>    
#include <MCUFRIEND_kbv.h>   

MCUFRIEND_kbv tft;

// 16-Bit Hex Color Palette
#define SCREEN_BG  0x0F0F  // Deep charcoal background
#define CARD_BG    0x1A5B  // Dark slate panels
#define ACCENT_BLU 0x05FF  // Neon cyan
#define ACCENT_YLW 0xFEE0  // Soft sun yellow
#define TEXT_WHT   0xFFFF  // Bright text
#define GRID_LINE  0x4444  // Subtle gray

// Variable simulation tracking
float currentTemp = 23.5;
int currentHumidity = 62;
int historyX = 40;
int lastGraphY = 270;

void setup() {
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486; 
    tft.begin(ID);
    
    tft.setRotation(1); // Set to wide Landscape mode
    tft.fillScreen(SCREEN_BG);
    
    drawStaticDashboard();
}

void loop() {
    // 1. Simulate changing sensor metrics
    currentTemp += random(-5, 6) * 0.1;
    currentHumidity += random(-2, 3);
    if(currentHumidity > 95) currentHumidity = 95;
    if(currentHumidity < 20) currentHumidity = 20;

    // 2. Refresh Readouts
    updateLiveMetrics();
    
    // 3. Append to historical trend plot
    updateHistoryGraph();
    
    delay(2000); // Sample update interval
}

void drawStaticDashboard() {
    // Header Banner
    tft.fillRect(0, 0, tft.width(), 40, CARD_BG);
    tft.setCursor(15, 12);
    tft.setTextColor(TEXT_WHT);
    tft.setTextSize(2);
    tft.print("MEGA WEATHER STATION");

    // Grid Dividers (Creating Panels)
    tft.drawFastHLine(0, 40, tft.width(), ACCENT_BLU);
    tft.drawFastVLine(240, 40, 160, GRID_LINE); // Vertical split top panel
    tft.drawFastHLine(0, 200, tft.width(), GRID_LINE); // Horizontal split for chart

    // Panel Labels
    tft.setTextColor(ACCENT_BLU);
    tft.setTextSize(1);
    tft.setCursor(15, 50);  
    tft.print("CURRENT CONDITIONS");
    tft.setCursor(255, 50); 
    tft.print("FORECAST");
    tft.setCursor(15, 210); 
    tft.print("24HR TEMP TREND CHART");
    
    // Draw Static Graph Axis
    tft.drawFastHLine(35, 280, tft.width() - 50, TEXT_WHT); // X-axis
    tft.drawFastVLine(35, 220, 60, TEXT_WHT);              // Y-axis
    
    // Draw a basic sun vector shape in forecast slot
    tft.fillCircle(350, 120, 22, ACCENT_YLW);
    for(int i = 0; i < 360; i += 45) {
        float angle = i * 0.0174532925;
        int x1 = 350 + cos(angle) * 28;
        int y1 = 120 + sin(angle) * 28;
        int x2 = 350 + cos(angle) * 36;
        int y2 = 120 + sin(angle) * 36;
        tft.drawLine(x1, y1, x2, y2, ACCENT_YLW);
    }
}

void updateLiveMetrics() {
    // Clear out past values using small custom background bounding boxes
    tft.fillRect(15, 80, 200, 95, SCREEN_BG);
    
    // Temperature Block
    tft.setTextColor(TEXT_WHT);
    tft.setTextSize(5);
    tft.setCursor(15, 80);
    tft.print(currentTemp, 1);
    tft.setTextSize(2);
    tft.print(" oC");
    
    // Humidity Block
    tft.setTextColor(ACCENT_BLU);
    tft.setTextSize(3);
    tft.setCursor(15, 140);
    tft.print("RH: ");
    tft.print(currentHumidity);
    tft.print("%");
}

void updateHistoryGraph() {
    // Map temperature range (15C to 35C) down into visual pixel window (y: 275 to 225)
    int graphY = map(currentTemp * 10, 150, 350, 275, 225);
    
    // Constrain inside graph borders
    if(graphY < 225) graphY = 225;
    if(graphY > 275) graphY = 275;

    // Plot a line connecting the previous point to the new point
    if (historyX > 40) {
        tft.drawLine(historyX - 10, lastGraphY, historyX, graphY, ACCENT_BLU);
    } else {
        tft.drawPixel(historyX, graphY, ACCENT_BLU);
    }
    
    lastGraphY = graphY;
    historyX += 10;
    
    // Reset graph layout if it wraps past display width boundaries
    if (historyX > tft.width() - 20) {
        historyX = 40;
        tft.fillRect(36, 220, tft.width() - 55, 59, SCREEN_BG); // Clear graph canvas area
    }
}
