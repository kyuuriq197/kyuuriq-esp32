#include "improvsetup.h"
#include "../config.h"
#include <ImprovWiFiLibrary.h>
#include <WiFi.h>
#include <Preferences.h>

static ImprovWiFi improv(&Serial);

static bool improvConnectWifi(const char* ssid, const char* password) {
  WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint8_t count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (count++ > 40) {
      return false;
    }
  }
  return true;
}

static void onImprovError(ImprovTypes::Error err) {
  Serial.print("Improv: error ");
  Serial.println((int)err);
}

static void onImprovConnected(const char* ssid, const char* password) {
  Preferences p;
  p.begin("wifi", false);
  p.putString("ssid", ssid ? ssid : "");
  p.putString("pass", password ? password : "");
  p.end();

  Serial.println("Improv: WiFi saved, rebooting...");
  delay(500);
  ESP.restart();
}

void improvBegin() {
#if defined(CONFIG_IDF_TARGET_ESP32S3)
  const ImprovTypes::ChipFamily cf = ImprovTypes::ChipFamily::CF_ESP32_S3;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
  const ImprovTypes::ChipFamily cf = ImprovTypes::ChipFamily::CF_ESP32_C3;
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
  const ImprovTypes::ChipFamily cf = ImprovTypes::ChipFamily::CF_ESP32_S2;
#else
  const ImprovTypes::ChipFamily cf = ImprovTypes::ChipFamily::CF_ESP32;
#endif

  improv.setDeviceInfo(cf, "KQ ESP", KQ_VERSION, "KQ ESP");
  improv.setCustomConnectWiFi(improvConnectWifi);
  improv.onImprovError(onImprovError);
  improv.onImprovConnected(onImprovConnected);
}

void improvLoop() {
  improv.handleSerial();
}
