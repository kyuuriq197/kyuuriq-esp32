#include "../../core/display.h"
#include "../../core/menu.h"
#include "../../config.h"

void settingsBrightness() {
  displayClear();
  displayTitle("Brightness");
  displayLine(0, "TODO", false);
}

void settingsPins() {
  displayClear();
  displayTitle("Pins");
  displayLine(0, "LED: " + String(LED_PIN), false);
  displayLine(1, "BTN UP: " + String(BTN_UP_PIN), false);
  displayLine(2, "BTN DOWN: " + String(BTN_DOWN_PIN), false);
  displayLine(3, "BTN OK: " + String(BTN_OK_PIN), false);
  displayLine(4, "BTN BACK: " + String(BTN_BACK_PIN), false);
}

void settingsReset() {
  displayClear();
  displayTitle("Reset Settings");
  displayLine(0, "TODO", false);
}

void settingsOpen() {
  static const MenuItem items[] = {
      {"Brightness", settingsBrightness},
      {"Pins", settingsPins},
      {"Reset", settingsReset},
  };
  menuRun({"Settings", items, 3});
}
