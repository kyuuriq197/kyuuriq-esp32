#include "../../core/display.h"
#include "../../core/menu.h"
#include <Wire.h>

void toolsI2cFinder() {
  displayClear();
  displayTitle("I2C Finder");
  Wire.begin();
  int found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      displayLine(found, String("0x") + String(addr, HEX), false);
      found++;
      if (found >= 9) break;
    }
  }
  if (found == 0) displayLine(0, "no devices", false);
}

void toolsPwmTest() {
  displayClear();
  displayTitle("PWM Test");
  displayLine(0, "TODO", false);
}

void toolsUptime() {
  displayClear();
  displayTitle("Uptime");
  unsigned long ms = millis();
  displayLine(0, String(ms / 60000) + "m " + String((ms / 1000) % 60) + "s", false);
}

void toolsOpen() {
  static const MenuItem items[] = {
      {"I2C Finder", toolsI2cFinder},
      {"PWM Test", toolsPwmTest},
      {"Uptime", toolsUptime},
  };
  menuRun({"Tools", items, 3});
}
