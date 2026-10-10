#include "../../core/display.h"
#include "../../core/menu.h"

void gpioLedModes() {
  displayClear();
  displayTitle("LED Modes");
  displayLine(0, "TODO: move setMode()", false);
}

void gpioPinOut() {
  displayClear();
  displayTitle("Pin Output");
  displayLine(0, "TODO", false);
}

void gpioPinRead() {
  displayClear();
  displayTitle("Pin Read");
  displayLine(0, "TODO", false);
}

void gpioOpen() {
  static const MenuItem items[] = {
      {"LED Modes", gpioLedModes},
      {"Pin Output", gpioPinOut},
      {"Pin Read", gpioPinRead},
  };
  menuRun({"GPIO / LED", items, 3});
}
