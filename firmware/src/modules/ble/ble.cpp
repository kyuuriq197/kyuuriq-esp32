#include "../../core/display.h"
#include "../../core/menu.h"

void bleScan() {
  displayClear();
  displayTitle("BLE Scan");
  displayLine(0, "TODO", false);
}

void bleKeyboard() {
  displayClear();
  displayTitle("BLE Keyboard");
  displayLine(0, "TODO", false);
}

void bleOpen() {
  static const MenuItem items[] = {
      {"Scan", bleScan},
      {"Keyboard (HID)", bleKeyboard},
  };
  menuRun({"BLE", items, 2});
}
