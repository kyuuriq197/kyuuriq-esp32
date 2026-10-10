#include "../../core/display.h"
#include "../../core/menu.h"

void wifiScan() {
  displayClear();
  displayTitle("WiFi Scan");
  displayLine(0, "TODO", false);
}

void wifiAp() {
  displayClear();
  displayTitle("Access Point");
  displayLine(0, "TODO", false);
}

void wifiWebUi() {
  displayClear();
  displayTitle("Web UI");
  displayLine(0, "TODO", false);
}

void wifiOpen() {
  static const MenuItem items[] = {
      {"Scan", wifiScan},
      {"Access Point", wifiAp},
      {"Web UI", wifiWebUi},
  };
  menuRun({"WiFi", items, 3});
}
