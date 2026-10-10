#include "wifisetup.h"
#include "improvsetup.h"
#include "../config.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

static WebServer setupServer(80);

static const char SETUP_PAGE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html lang="en"><head><meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>KQ ESP Setup</title><style>
body{font-family:Inter,-apple-system,Segoe UI,Arial,sans-serif;background:#0b0f17;
color:#f5f7fa;margin:0;padding:36px 18px}
.container{max-width:460px;margin:auto}
h1{font-size:24px;margin:0 0 6px}.sub{color:#8b98ac;margin-bottom:24px}
.card{background:#141b29;border:1px solid #23304a;border-radius:14px;padding:20px}
label{display:block;margin:12px 0 6px;color:#8b98ac;font-size:14px}
input{width:100%;box-sizing:border-box;padding:12px;border-radius:9px;
border:1px solid #23304a;background:#0b0f17;color:#f5f7fa;font-size:15px}
button{margin-top:18px;width:100%;padding:13px;border:0;border-radius:9px;
background:#65d6a6;color:#052015;font-weight:700;font-size:15px}
</style></head><body><div class="container">
<h1>KQ ESP</h1><div class="sub">Wi-Fi setup &middot; )HTML";

static const char SETUP_FORM[] PROGMEM = R"HTML(
</div><div class="card">
<form method="POST" action="/save">
<label>Wi-Fi name (SSID)</label>
<input name="ssid" required maxlength="32" placeholder="MyNetwork">
<label>Wi-Fi password</label>
<input name="pass" type="password" maxlength="64" placeholder="********">
<button type="submit">Save &amp; Reboot</button>
</form></div>
<p style="color:#8b98ac;font-size:13px;margin-top:16px">
Connect your phone/PC to Wi-Fi "KQ-ESP-Setup", open
http://192.168.4.1 and enter your home Wi-Fi.</p>
</div></body></html>)HTML";

static void handleForm() {
  String page = FPSTR(SETUP_PAGE);
  page += KQ_VERSION;
  page += FPSTR(SETUP_FORM);
  setupServer.send(200, "text/html", page);
}

static void handleSave() {
  if (!setupServer.hasArg("ssid")) {
    setupServer.send(400, "text/plain", "Missing ssid");
    return;
  }
  Preferences p;
  p.begin("wifi", false);
  p.putString("ssid", setupServer.arg("ssid"));
  p.putString("pass", setupServer.arg("pass"));
  p.end();

  setupServer.send(200, "text/html",
    "<body style=\"font-family:sans-serif;background:#0b0f17;color:#f5f7fa;"
    "text-align:center;padding-top:80px\"><h2>Saved!</h2>"
    "<p>Rebooting and connecting...</p></body>");
  delay(1500);
  ESP.restart();
}

static void handleNotFound() {
  handleForm();
}

void wifiSetupPortal() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("KQ-ESP-Setup");

  setupServer.on("/", HTTP_GET, handleForm);
  setupServer.on("/save", HTTP_POST, handleSave);
  setupServer.onNotFound(handleNotFound);
  setupServer.begin();

  Serial.println();
  Serial.println("=== Wi-Fi SETUP MODE ===");
  Serial.println("AP: KQ-ESP-Setup");
  Serial.print("Open http://");
  Serial.println(WiFi.softAPIP());

  while (true) {
    setupServer.handleClient();
    improvLoop();
    delay(2);
  }
}

bool wifiLoadCreds(String& ssid, String& pass) {
  Preferences p;
  p.begin("wifi", true);
  ssid = p.getString("ssid", "");
  pass = p.getString("pass", "");
  p.end();
  return ssid.length() > 0;
}
