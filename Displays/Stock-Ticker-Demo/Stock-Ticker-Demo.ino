#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>

MCUFRIEND_kbv tft;

// 16-Bit Hex Color Palette
#define SCREEN_BG  0x0000  // Black background (classic stock board)
#define CARD_BG    0x18E3  // Dark slate panels
#define ACCENT_BLU 0x05FF  // Neon cyan
#define ACCENT_GRN 0x07E0  // Gain green
#define ACCENT_RED 0xF800  // Loss red
#define TEXT_WHT   0xFFFF  // Bright text
#define GRID_LINE  0x4444  // Subtle gray

// ---- Simulated FANG market data ----
// Each stock has a run of "snapshots" the display cycles through
// to fake a live feed without any real network connection.
#define NUM_STOCKS   4
#define NUM_SNAPS    6

struct Stock {
    const char *symbol;
    float prices[NUM_SNAPS];
};

Stock stocks[NUM_STOCKS] = {
    { "META",  { 468.20, 471.55, 465.80, 473.10, 469.95, 476.40 } },
    { "AMZN",  { 186.40, 188.10, 184.75, 187.90, 190.20, 189.55 } },
    { "NFLX",  { 625.10, 631.40, 618.75, 622.30, 629.80, 634.15 } },
    { "GOOGL", { 162.35, 164.80, 160.90, 163.55, 166.20, 165.40 } },
};

int tickIndex = 0; // which snapshot is "current" right now

unsigned long lastPriceUpdate = 0;
unsigned long lastClockTick = 0;
const unsigned long PRICE_UPDATE_MS = 3000; // simulate a new "tick" every 3s
const unsigned long CLOCK_STEP_MS   = 1000; // redraw clock once a second

// ---- Upload-time clock ----
// __TIME__ is the compiler's build timestamp, i.e. roughly "when the
// sketch was uploaded". We parse it once, then count elapsed seconds
// with millis() to keep a running clock with no RTC hardware needed.
unsigned long clockStartSecOfDay = 0; // seconds since midnight at upload
unsigned long clockStartMillis = 0;
int clockAreaX = 0; // top-right x position, computed once screen width is known

// Layout
int cardTop;
int cardH;
int cardW;

void setup() {
    Serial.begin(9600);

    uint16_t ID = tft.readID();
    if (ID == 0xD3D3) ID = 0x9486;
    tft.begin(ID);

    tft.setRotation(1); // wide landscape, "stock board" style
    tft.fillScreen(SCREEN_BG);

    cardTop = 50;
    cardH = (tft.height() - cardTop) / 2;
    cardW = tft.width() / 2;
    clockAreaX = tft.width() - 100;

    parseUploadTime();

    drawStaticDashboard();
    drawStockCards();
    drawClock();

    lastPriceUpdate = millis();
    lastClockTick = millis();
}

void loop() {
    unsigned long now = millis();

    // 1. Advance to the next canned snapshot to fake a live price feed
    if (now - lastPriceUpdate >= PRICE_UPDATE_MS) {
        lastPriceUpdate = now;
        tickIndex = (tickIndex + 1) % NUM_SNAPS;
        drawStockCards();
    }

    // 2. Tick the upload-time clock in the top-right corner
    if (now - lastClockTick >= CLOCK_STEP_MS) {
        lastClockTick = now;
        drawClock();
    }
}

void parseUploadTime() {
    // __TIME__ is baked in by the compiler as "HH:MM:SS" at build time,
    // which lines up closely with when the sketch gets uploaded to the board.
    int h = (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0');
    int m = (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0');
    int s = (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0');

    clockStartSecOfDay = (unsigned long)h * 3600UL + (unsigned long)m * 60UL + s;
    clockStartMillis = millis();
}

void drawClock() {
    unsigned long elapsedSec = (millis() - clockStartMillis) / 1000UL;
    unsigned long secOfDay = (clockStartSecOfDay + elapsedSec) % 86400UL;

    int h = secOfDay / 3600UL;
    int m = (secOfDay % 3600UL) / 60UL;
    int s = secOfDay % 60UL;

    char buf[9];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);

    tft.fillRect(clockAreaX, 0, tft.width() - clockAreaX, 40, CARD_BG);
    tft.setCursor(clockAreaX + 5, 12);
    tft.setTextColor(TEXT_WHT);
    tft.setTextSize(2);
    tft.print(buf);
}

void drawStaticDashboard() {
    // Header Banner
    tft.fillRect(0, 0, tft.width(), 40, CARD_BG);
    tft.setCursor(15, 12);
    tft.setTextColor(ACCENT_GRN);
    tft.setTextSize(2);
    tft.print("FANG MARKET WATCH");

    tft.drawFastHLine(0, 40, tft.width(), ACCENT_BLU);

    // Divider between the 4 quote cards
    tft.drawFastVLine(cardW, cardTop, cardH * 2, GRID_LINE);
    tft.drawFastHLine(0, cardTop + cardH, tft.width(), GRID_LINE);
}

void drawStockCards() {
    for (int i = 0; i < NUM_STOCKS; i++) {
        int col = i % 2;
        int row = i / 2;
        int x = col * cardW;
        int y = cardTop + row * cardH;

        // Clear just this card's interior (leave the grid lines alone)
        tft.fillRect(x + 2, y + 2, cardW - 4, cardH - 4, SCREEN_BG);

        float price = stocks[i].prices[tickIndex];
        int prevIdx = (tickIndex - 1 + NUM_SNAPS) % NUM_SNAPS;
        float change = price - stocks[i].prices[prevIdx];
        bool up = change >= 0;
        uint16_t changeColor = up ? ACCENT_GRN : ACCENT_RED;

        // Symbol
        tft.setTextColor(TEXT_WHT);
        tft.setTextSize(2);
        tft.setCursor(x + 15, y + 10);
        tft.print(stocks[i].symbol);

        // Price
        tft.setTextColor(TEXT_WHT);
        tft.setTextSize(3);
        tft.setCursor(x + 15, y + 35);
        tft.print(price, 2);

        // Change with up/down arrow
        tft.setTextColor(changeColor);
        tft.setTextSize(2);
        tft.setCursor(x + 15, y + 70);
        tft.print(up ? "^ " : "v ");
        tft.print(fabs(change), 2);
    }
}
