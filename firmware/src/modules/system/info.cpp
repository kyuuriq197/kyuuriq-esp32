#include "../../core/display.h"
#include "../../core/menu.h"
#include "../../config.h"
#include <esp_heap_caps.h>
#include <WiFi.h>

void infoOpen() {
  displayClear();
  displayTitle("About");
  displayLine(0, "KQ ESP " KQ_VERSION, false);
  displayLine(1, "Heap: " + String(ESP.getFreeHeap() / 1024) + " KB", false);
  displayLine(2, "Flash: " + String(ESP.getFlashChipSize() / (1024 * 1024)) + " MB", false);
  displayLine(3, "PSRAM: " + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024) + " KB", false);
  displayLine(4, "MAC: " + String(WiFi.macAddress()), false);
}
