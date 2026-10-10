#include "webmenu.h"
#include "../config.h"
#include <WiFi.h>
#include <Wire.h>
#include <esp_heap_caps.h>

static WebServer* web = nullptr;

static String htmlOpen(const String& title) {
  String s;
  s += F("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">");
  s += F("<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">");
  s += F("<title>");
  s += title;
  s += F("</title><style>");
  s += F("body{font-family:Inter,-apple-system,Segoe UI,Arial,sans-serif;");
  s += F("background:#0b0f17;color:#f5f7fa;margin:0;padding:28px 16px}");
  s += F(".container{max-width:720px;margin:auto}");
  s += F("h1{font-size:22px;margin:0 0 6px}.sub{color:#8b98ac;margin-bottom:22px}");
  s += F(".card{background:#141b29;border:1px solid #23304a;border-radius:14px;");
  s += F("padding:16px;margin-bottom:12px}");
  s += F("a.card{display:block;text-decoration:none;color:inherit}");
  s += F(".name{font-weight:600}a.btn{display:inline-block;margin:6px 6px 0 0;");
  s += F("padding:9px 14px;border-radius:9px;background:#65d6a6;color:#052015;");
  s += F("text-decoration:none;font-weight:600;font-size:14px}");
  s += F(".row{display:flex;justify-content:space-between;border-bottom:1px solid #23304a;");
  s += F("padding:7px 0}.k{color:#8b98ac}.v{font-weight:600}");
  s += F(".back{color:#65d6a6;text-decoration:none;font-size:14px}");
  s += F("</style></head><body><div class=\"container\">");
  s += "<h1>KQ ESP</h1><div class=\"sub\">" + title + " · " + KQ_VERSION + "</div>";
  return s;
}

static String htmlClose() {
  return F("<p style=\"margin-top:22px\"><a class=\"back\" href=\"/menu\">&larr; Menu</a> · <a class=\"back\" href=\"/\">LED UI</a></p></div></body></html>");
}

static void handleMenuRoot() {
  String s = htmlOpen("Web Menu");
  s += F("<a class=\"card\" href=\"/menu/wifi\"><div class=\"name\">WiFi Scan</div><div class=\"sub\" style=\"margin:4px 0 0\">scan nearby networks</div></a>");
  s += F("<a class=\"card\" href=\"/menu/i2c\"><div class=\"name\">I2C Finder</div><div class=\"sub\" style=\"margin:4px 0 0\">list devices on the I2C bus</div></a>");
  s += F("<a class=\"card\" href=\"/menu/system\"><div class=\"name\">System Info</div><div class=\"sub\" style=\"margin:4px 0 0\">heap, flash, PSRAM, MAC, uptime</div></a>");
  s += htmlClose();
  web->send(200, "text/html", s);
}

static void handleWifi() {
  String s = htmlOpen("WiFi Scan");
  int n = WiFi.scanNetworks();
  s += "<div class=\"card\">";
  if (n <= 0) {
    s += F("<div class=\"name\">No networks found</div>");
  } else {
    for (int i = 0; i < n && i < 30; i++) {
      s += "<div class=\"row\"><span class=\"k\">" + WiFi.SSID(i) + "</span><span class=\"v\">" +
           String(WiFi.RSSI(i)) + " dBm</span></div>";
    }
  }
  s += "</div>";
  WiFi.scanDelete();
  s += htmlClose();
  web->send(200, "text/html", s);
}

static void handleI2c() {
  String s = htmlOpen("I2C Finder");
  Wire.begin();
  s += "<div class=\"card\">";
  int found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      s += "<div class=\"row\"><span class=\"k\">Device</span><span class=\"v\">0x" +
           String(addr, HEX) + "</span></div>";
      found++;
    }
  }
  if (found == 0) s += F("<div class=\"name\">No devices found</div>");
  s += "</div>";
  s += htmlClose();
  web->send(200, "text/html", s);
}

static void handleSystem() {
  String s = htmlOpen("System Info");
  unsigned long ms = millis();
  String up = String(ms / 60000) + "m " + String((ms / 1000) % 60) + "s";
  s += "<div class=\"card\">";
  s += "<div class=\"row\"><span class=\"k\">Version</span><span class=\"v\">" + String(KQ_VERSION) + "</span></div>";
  s += "<div class=\"row\"><span class=\"k\">Uptime</span><span class=\"v\">" + up + "</span></div>";
  s += "<div class=\"row\"><span class=\"k\">Free heap</span><span class=\"v\">" + String(ESP.getFreeHeap() / 1024) + " KB</span></div>";
  s += "<div class=\"row\"><span class=\"k\">Flash</span><span class=\"v\">" + String(ESP.getFlashChipSize() / (1024 * 1024)) + " MB</span></div>";
  s += "<div class=\"row\"><span class=\"k\">PSRAM free</span><span class=\"v\">" + String(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024) + " KB</span></div>";
  s += "<div class=\"row\"><span class=\"k\">MAC</span><span class=\"v\">" + WiFi.macAddress() + "</span></div>";
  s += "</div>";
  s += htmlClose();
  web->send(200, "text/html", s);
}

void webMenuBegin(WebServer& server) {
  web = &server;
  server.on("/menu", HTTP_GET, handleMenuRoot);
  server.on("/menu/wifi", HTTP_GET, handleWifi);
  server.on("/menu/i2c", HTTP_GET, handleI2c);
  server.on("/menu/system", HTTP_GET, handleSystem);
}
