#include "display.h"
#include "../config.h"

#if __has_include(<Adafruit_ST7789.h>)
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#define KQ_HAS_TFT 1
static Adafruit_ST7789 tft(10, 11, 12);
#endif

static const int ROW_HEIGHT = 20;
static const int MARGIN = 8;

void displaySetup() {
#ifdef KQ_HAS_TFT
  tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  tft.setRotation(0);
  tft.fillScreen(ST7735_BLACK);
#endif
}

void displayClear() {
#ifdef KQ_HAS_TFT
  tft.fillScreen(ST7735_BLACK);
#endif
}

void displayTitle(const String& title) {
#ifdef KQ_HAS_TFT
  tft.fillRect(0, 0, DISPLAY_WIDTH, ROW_HEIGHT + MARGIN, ST7735_BLUE);
  tft.setCursor(MARGIN, MARGIN / 2 + 2);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.print(title);
#endif
}

void displayLine(int row, const String& text, bool selected) {
#ifdef KQ_HAS_TFT
  int y = (ROW_HEIGHT + MARGIN) + row * ROW_HEIGHT;
  tft.fillRect(0, y, DISPLAY_WIDTH, ROW_HEIGHT,
               selected ? ST7735_WHITE : ST7735_BLACK);
  tft.setCursor(MARGIN, y + 6);
  tft.setTextColor(selected ? ST7735_BLACK : ST7735_WHITE);
  tft.setTextSize(1);
  tft.print(text);
#endif
}

void displayStatus(const String& left, const String& right) {
#ifdef KQ_HAS_TFT
  int y = DISPLAY_HEIGHT - ROW_HEIGHT - MARGIN;
  tft.fillRect(0, y, DISPLAY_WIDTH, ROW_HEIGHT, ST7735_DARKGREY);
  tft.setCursor(MARGIN, y + 6);
  tft.setTextColor(ST7735_WHITE);
  tft.print(left);
  int w = right.length() * 6;
  tft.setCursor(DISPLAY_WIDTH - w - MARGIN, y + 6);
  tft.print(right);
#endif
}

void displaySplash(const String& version) {
  displayClear();
#ifdef KQ_HAS_TFT
  tft.setCursor(MARGIN, DISPLAY_HEIGHT / 2 - 10);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print("KQ ESP");
  tft.setTextSize(1);
  tft.setCursor(MARGIN, DISPLAY_HEIGHT / 2 + 16);
  tft.print(version);
#endif
}
