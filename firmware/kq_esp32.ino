#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <Preferences.h>
#include "secrets.h"
#include "src/config.h"
#include "src/core/mainmenu.h"


// ============================================================
// KYUURIQ ESP v0.5.0
// ESP32-WROOM-32
// ============================================================


// -------------------- CONFIG --------------------

#define LED_PIN 2

#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8


// -------------------- LED MODES --------------------

enum LEDMode {
  MODE_SOLID,
  MODE_BLINK,
  MODE_FAST_BLINK,
  MODE_SLOW_BLINK,
  MODE_PULSE,
  MODE_BREATHING
};


// -------------------- OBJECTS --------------------

WebServer server(80);

Preferences preferences;


// -------------------- LED STATE --------------------

bool ledState = false;

uint8_t brightness = 100;

LEDMode ledMode = MODE_SOLID;


// -------------------- EFFECT STATE --------------------

unsigned long lastEffectUpdate = 0;

bool blinkState = false;

int effectValue = 0;

int effectDirection = 1;

// -------------------- SERIAL --------------------

String command = "";


// ============================================================
// MODE NAMES
// ============================================================

const char* modeName(LEDMode mode) {

  switch (mode) {

    case MODE_SOLID:
      return "Solid";

    case MODE_BLINK:
      return "Blink";

    case MODE_FAST_BLINK:
      return "Fast Blink";

    case MODE_SLOW_BLINK:
      return "Slow Blink";

    case MODE_PULSE:
      return "Pulse";

    case MODE_BREATHING:
      return "Breathing";

    default:
      return "Solid";
  }

}


// ============================================================
// MODE FROM STRING
// ============================================================

LEDMode modeFromString(String value) {

  value.toLowerCase();

  if (value == "solid") {
    return MODE_SOLID;
  }

  if (value == "blink") {
    return MODE_BLINK;
  }

  if (value == "fast") {
    return MODE_FAST_BLINK;
  }

  if (value == "slow") {
    return MODE_SLOW_BLINK;
  }

  if (value == "pulse") {
    return MODE_PULSE;
  }

  if (value == "breathing") {
    return MODE_BREATHING;
  }

  return MODE_SOLID;
}


// ============================================================
// SAVE SETTINGS
// ============================================================

void saveSettings() {

  preferences.begin(
    "kyuuriq",
    false
  );


  preferences.putBool(
    "led",
    ledState
  );


  preferences.putUChar(
    "brightness",
    brightness
  );


  preferences.putUChar(
    "mode",
    (uint8_t)ledMode
  );


  preferences.end();

}


// ============================================================
// LOAD SETTINGS
// ============================================================

void loadSettings() {

  preferences.begin(
    "kyuuriq",
    true
  );


  ledState =
    preferences.getBool(
      "led",
      false
    );


  brightness =
    preferences.getUChar(
      "brightness",
      100
    );


  uint8_t savedMode =
    preferences.getUChar(
      "mode",
      MODE_SOLID
    );


  if (savedMode > MODE_BREATHING) {
    savedMode = MODE_SOLID;
  }


  ledMode =
    (LEDMode)savedMode;


  preferences.end();

}


// ============================================================
// PWM SETUP
// ============================================================

void setupPWM() {

  ledcAttach(
    LED_PIN,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

}


// ============================================================
// WRITE LED
// ============================================================

void writeLED(uint8_t value) {

  /*
    GPIO 2 onboard LED is ACTIVE-LOW.

    255 = HIGH = OFF
    0   = LOW  = MAXIMUM ON
  */

  uint8_t duty =
    255 - value;


  ledcWrite(
    LED_PIN,
    duty
  );

}


// ============================================================
// APPLY SOLID BRIGHTNESS
// ============================================================

void applyLED() {

  if (
    !ledState ||
    brightness == 0
  ) {

    writeLED(0);

    return;
  }


  writeLED(
    brightness
  );

}


// ============================================================
// RESET EFFECT
// ============================================================

void resetEffect() {

  lastEffectUpdate = millis();

  blinkState = false;

  effectValue = 0;

  effectDirection = 1;


  /*
    Start pulse/breathing from a sensible state.
  */

  if (ledMode == MODE_PULSE ||
      ledMode == MODE_BREATHING) {

    effectValue = 0;
    effectDirection = 1;

  }

}


// ============================================================
// SET LED STATE
// ============================================================

void setLED(bool state) {

  ledState = state;


  /*
    If ON is selected while brightness is 0,
    restore maximum brightness.
  */

  if (
    ledState &&
    brightness == 0
  ) {

    brightness = 100;

  }


  resetEffect();

  applyLED();

  saveSettings();

}


// ============================================================
// SET BRIGHTNESS
// ============================================================

void setBrightness(uint8_t value) {

  if (value > 100) {
    value = 100;
  }


  brightness = value;


  if (brightness == 0) {

    ledState = false;

  }

  else {

    ledState = true;

  }


  resetEffect();

  applyLED();

  saveSettings();

}


// ============================================================
// SET MODE
// ============================================================

void setMode(LEDMode mode) {

  ledMode = mode;


  resetEffect();


  /*
    Immediately show the selected mode.
  */

  if (ledState) {

    if (ledMode == MODE_SOLID) {

      applyLED();

    }

    else {

      updateEffect();

    }

  }


  saveSettings();

}


// ============================================================
// EFFECT UPDATE
// ============================================================

void updateEffect() {

  if (!ledState || brightness == 0) {

    writeLED(0);

    return;

  }


  unsigned long now = millis();


  // ==========================================================
  // SOLID
  // ==========================================================

  if (ledMode == MODE_SOLID) {

    writeLED(
      brightness
    );

    return;

  }


  // ==========================================================
  // BLINK
  // ==========================================================

  if (ledMode == MODE_BLINK) {

    if (
      now - lastEffectUpdate >= 500
    ) {

      lastEffectUpdate = now;

      blinkState = !blinkState;

    }


    if (blinkState) {

      writeLED(
        brightness
      );

    }

    else {

      writeLED(0);

    }

    return;

  }


  // ==========================================================
  // FAST BLINK
  // ==========================================================

  if (ledMode == MODE_FAST_BLINK) {

    if (
      now - lastEffectUpdate >= 120
    ) {

      lastEffectUpdate = now;

      blinkState = !blinkState;

    }


    if (blinkState) {

      writeLED(
        brightness
      );

    }

    else {

      writeLED(0);

    }

    return;

  }


  // ==========================================================
  // SLOW BLINK
  // ==========================================================

  if (ledMode == MODE_SLOW_BLINK) {

    if (
      now - lastEffectUpdate >= 1000
    ) {

      lastEffectUpdate = now;

      blinkState = !blinkState;

    }


    if (blinkState) {

      writeLED(
        brightness
      );

    }

    else {

      writeLED(0);

    }

    return;

  }


  // ==========================================================
  // PULSE
  // ==========================================================

  if (ledMode == MODE_PULSE) {

    if (
      now - lastEffectUpdate >= 15
    ) {

      lastEffectUpdate = now;


      effectValue +=
        effectDirection * 5;


      if (effectValue >= 100) {

        effectValue = 100;

        effectDirection = -1;

      }


      if (effectValue <= 0) {

        effectValue = 0;

        effectDirection = 1;

      }

    }


    uint8_t output =
      (brightness * effectValue) / 100;


    writeLED(
      output
    );

    return;

  }


  // ==========================================================
  // BREATHING
  // ==========================================================

  if (ledMode == MODE_BREATHING) {

    if (
      now - lastEffectUpdate >= 25
    ) {

      lastEffectUpdate = now;


      effectValue +=
        effectDirection * 2;


      if (effectValue >= 100) {

        effectValue = 100;

        effectDirection = -1;

      }


      if (effectValue <= 0) {

        effectValue = 0;

        effectDirection = 1;

      }

    }


    /*
      Smooth breathing curve.

      The sine-like curve makes the transition
      less linear and more natural.
    */

    float x =
      effectValue / 100.0;


    float curve =
      (1.0 - cos(x * PI)) / 2.0;


    uint8_t output =
      (uint8_t)(
        brightness * curve
      );


    writeLED(
      output
    );

    return;

  }

}


// ============================================================
// HTML PAGE
// ============================================================

String makePage() {

  String html = R"rawliteral(

<!DOCTYPE html>

<html lang="en">

<head>

<meta charset="UTF-8">

<meta
  name="viewport"
  content="width=device-width, initial-scale=1.0"
>

<meta
  http-equiv="refresh"
  content="10"
>

<title>KYUURIQ ESP</title>


<style>

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}


body {

  min-height: 100vh;

  font-family:
    Inter,
    -apple-system,
    BlinkMacSystemFont,
    "Segoe UI",
    Arial,
    sans-serif;

  background:
    radial-gradient(
      circle at top,
      #172033 0%,
      #0b0f17 45%,
      #06080d 100%
    );

  color: #f5f7fa;

  padding: 30px 18px;

}


.container {

  width: 100%;

  max-width: 720px;

  margin: auto;

}


.header {

  text-align: center;

  padding: 25px 10px 30px;

}


.badge {

  display: inline-block;

  padding: 7px 14px;

  border: 1px solid #303b4f;

  border-radius: 999px;

  color: #9fb2cc;

  font-size: 13px;

  background: rgba(255,255,255,0.03);

  margin-bottom: 20px;

}


h1 {

  font-size: clamp(
    42px,
    10vw,
    68px
  );

  letter-spacing: -3px;

  line-height: 1;

  margin-bottom: 15px;

}


.accent {

  color: #65d6a6;

}


.subtitle {

  color: #aab5c5;

  font-size: 17px;

}


.card {

  background:
    rgba(17, 23, 34, 0.90);

  border:
    1px solid #263044;

  border-radius: 18px;

  padding: 24px;

  margin-bottom: 16px;

  box-shadow:
    0 25px 70px rgba(0,0,0,0.30);

}


.section-title {

  font-size: 13px;

  color: #7f8b9c;

  text-transform: uppercase;

  letter-spacing: 1.5px;

  margin-bottom: 17px;

}


.status {

  display: flex;

  align-items: center;

  justify-content: space-between;

  padding: 14px 16px;

  background: #090d14;

  border:
    1px solid #222b3b;

  border-radius: 12px;

}


.status-left {

  display: flex;

  align-items: center;

  gap: 10px;

}


.dot {

  width: 9px;

  height: 9px;

  border-radius: 50%;

  background: #65d6a6;

  box-shadow:
    0 0 12px rgba(
      101,
      214,
      166,
      0.7
    );

}


.online {

  color: #65d6a6;

  font-weight: 700;

}


.ip {

  color: #68758a;

  font-family: monospace;

  font-size: 13px;

}


.led-status {

  text-align: center;

  padding: 25px 10px;

  margin-bottom: 20px;

  border-bottom:
    1px solid #222b3b;

}


.led-label {

  color: #7f8b9c;

  font-size: 12px;

  text-transform: uppercase;

  letter-spacing: 1.5px;

  margin-bottom: 8px;

}


.led-value {

  font-size: 32px;

  font-weight: 800;

}


.on {

  color: #65d6a6;

}


.off {

  color: #8791a0;

}


.buttons {

  display: grid;

  grid-template-columns:
    1fr 1fr;

  gap: 10px;

}


button {

  border: none;

  border-radius: 11px;

  padding: 15px;

  font-size: 15px;

  font-weight: 700;

  cursor: pointer;

  transition:
    transform 0.15s,
    filter 0.15s;

}


button:hover {

  filter: brightness(1.1);

  transform:
    translateY(-2px);

}


button:active {

  transform:
    translateY(0);

}


.green {

  background: #65d6a6;

  color: #07110c;

}


.dark {

  background: #252d38;

  color: white;

  border:
    1px solid #354158;

}


.slider {

  width: 100%;

  accent-color: #65d6a6;

  margin:
    18px 0;

}


.brightness-value {

  text-align: center;

  font-size: 26px;

  font-weight: 800;

  color: #65d6a6;

}


.mode-select {

  width: 100%;

  padding: 14px 15px;

  border-radius: 11px;

  border: 1px solid #354158;

  background: #090d14;

  color: #f5f7fa;

  font-size: 15px;

  font-weight: 600;

  outline: none;

  cursor: pointer;

}


.mode-select:focus {

  border-color: #65d6a6;

}


.mode-description {

  color: #7f8b9c;

  font-size: 13px;

  margin-top: 12px;

  line-height: 1.5;

}


.system-row {

  display: flex;

  justify-content: space-between;

  padding: 11px 0;

  border-bottom:
    1px solid #1d2533;

}


.system-row:last-child {

  border-bottom: none;

}


.system-name {

  color: #7f8b9c;

}


.system-value {

  color: #dce3ec;

  font-family: monospace;

}


.footer {

  text-align: center;

  color: #68758a;

  font-size: 12px;

  padding:
    10px 0 20px;

}


@media (
  max-width: 500px
) {

  .card {
    padding: 18px;
  }

  .ip {
    font-size: 11px;
  }

}

</style>

</head>


<body>


<div class="container">


<div class="header">

  <div class="badge">
    ESP32-WROOM-32 · LOCAL
  </div>

  <h1>
    KYUURIQ
    <span class="accent">
      ESP
    </span>
  </h1>

  <p class="subtitle">
    Local device control
  </p>

</div>


<!-- DEVICE -->

<div class="card">

  <div class="section-title">
    Device
  </div>


  <div class="status">

    <div class="status-left">

      <span class="dot"></span>

      <span class="online">
        ONLINE
      </span>

    </div>


    <span class="ip">
      %IP%
    </span>

  </div>

</div>


<!-- LED -->

<div class="card">

  <div class="section-title">
    LED Control
  </div>


  <div class="led-status">

    <div class="led-label">
      STATUS
    </div>

    <div class="led-value %LEDCLASS%">
      %LED%
    </div>

  </div>


  <div class="buttons">

    <button
      class="green"
      onclick="location.href='/on'">
      ON
    </button>


    <button
      class="dark"
      onclick="location.href='/off'">
      OFF
    </button>

  </div>

</div>


<!-- BRIGHTNESS -->

<div class="card">

  <div class="section-title">
    PWM / Brightness
  </div>


  <div class="brightness-value">
    %BRIGHTNESS%%
  </div>


  <input
    class="slider"
    type="range"
    min="0"
    max="100"
    value="%BRIGHTNESS%"
    oninput="document.getElementById('bv').innerText=this.value+'%'"
    onchange="location.href='/brightness?value='+this.value"
  >


  <div
    id="bv"
    style="
      text-align:center;
      color:#7f8b9c;
      font-size:13px;
    "
  >
    0% = OFF · 100% = MAX
  </div>

</div>


<!-- MODES -->

<div class="card">

  <div class="section-title">
    LED Modes
  </div>


  <select
    class="mode-select"
    onchange="location.href='/mode?value='+this.value"
  >

    <option
      value="solid"
      %SOLID%
    >
      Solid
    </option>


    <option
      value="blink"
      %BLINK%
    >
      Blink
    </option>


    <option
      value="fast"
      %FAST%
    >
      Fast Blink
    </option>


    <option
      value="slow"
      %SLOW%
    >
      Slow Blink
    </option>


    <option
      value="pulse"
      %PULSE%
    >
      Pulse
    </option>


    <option
      value="breathing"
      %BREATHING%
    >
      Breathing
    </option>

  </select>


  <div class="mode-description">

    Current mode:
    <strong>%MODE%</strong>

  </div>

</div>


<!-- SYSTEM -->

<div class="card">

  <div class="section-title">
    System
  </div>


  <div class="system-row">

    <span class="system-name">
      Free RAM
    </span>

    <span class="system-value">
      %RAM% bytes
    </span>

  </div>


  <div class="system-row">

    <span class="system-name">
      Uptime
    </span>

    <span class="system-value">
      %UPTIME% sec
    </span>

  </div>


  <div class="system-row">

    <span class="system-name">
      Wi-Fi
    </span>

    <span class="system-value">
      %RSSI% dBm
    </span>

  </div>


  <div class="system-row">

    <span class="system-name">
      Mode
    </span>

    <span class="system-value">
      %MODE%
    </span>

  </div>


  <div class="system-row">

    <span class="system-name">
      Firmware
    </span>

    <span class="system-value">
      v0.5.0
    </span>

  </div>

</div>


<div class="footer">

  KYUURIQ ESP · LOCAL · v0.5.0

</div>


</div>


</body>

</html>

)rawliteral";


  // ==========================================================
  // LED
  // ==========================================================

  html.replace(
    "%LED%",
    ledState ? "ON" : "OFF"
  );


  html.replace(
    "%LEDCLASS%",
    ledState ? "on" : "off"
  );


  // ==========================================================
  // BRIGHTNESS
  // ==========================================================

  html.replace(
    "%BRIGHTNESS%",
    String(brightness)
  );


  // ==========================================================
  // MODE
  // ==========================================================

  html.replace(
    "%MODE%",
    modeName(ledMode)
  );


  String solid =
    ledMode == MODE_SOLID
      ? "selected"
      : "";


  String blink =
    ledMode == MODE_BLINK
      ? "selected"
      : "";


  String fast =
    ledMode == MODE_FAST_BLINK
      ? "selected"
      : "";


  String slow =
    ledMode == MODE_SLOW_BLINK
      ? "selected"
      : "";


  String pulse =
    ledMode == MODE_PULSE
      ? "selected"
      : "";


  String breathing =
    ledMode == MODE_BREATHING
      ? "selected"
      : "";


  html.replace(
    "%SOLID%",
    solid
  );


  html.replace(
    "%BLINK%",
    blink
  );


  html.replace(
    "%FAST%",
    fast
  );


  html.replace(
    "%SLOW%",
    slow
  );


  html.replace(
    "%PULSE%",
    pulse
  );


  html.replace(
    "%BREATHING%",
    breathing
  );


  // ==========================================================
  // SYSTEM
  // ==========================================================

  html.replace(
    "%IP%",
    WiFi.localIP().toString()
  );


  html.replace(
    "%RAM%",
    String(ESP.getFreeHeap())
  );


  html.replace(
    "%UPTIME%",
    String(millis() / 1000)
  );


  html.replace(
    "%RSSI%",
    String(WiFi.RSSI())
  );


  return html;

}


// ============================================================
// WEB ROOT
// ============================================================

void handleRoot() {

  server.send(
    200,
    "text/html",
    makePage()
  );

}


// ============================================================
// WEB ON
// ============================================================

void handleOn() {

  setLED(true);


  server.sendHeader(
    "Location",
    "/"
  );


  server.send(303);

}


// ============================================================
// WEB OFF
// ============================================================

void handleOff() {

  setLED(false);


  server.sendHeader(
    "Location",
    "/"
  );


  server.send(303);

}


// ============================================================
// WEB BRIGHTNESS
// ============================================================

void handleBrightness() {

  if (
    !server.hasArg("value")
  ) {

    server.send(
      400,
      "text/plain",
      "Missing value"
    );

    return;

  }


  int value =
    server.arg("value").toInt();


  value =
    constrain(
      value,
      0,
      100
    );


  setBrightness(
    value
  );


  server.sendHeader(
    "Location",
    "/"
  );


  server.send(303);

}


// ============================================================
// WEB MODE
// ============================================================

void handleMode() {

  if (
    !server.hasArg("value")
  ) {

    server.send(
      400,
      "text/plain",
      "Missing mode"
    );

    return;

  }


  LEDMode newMode =
    modeFromString(
      server.arg("value")
    );


  setMode(
    newMode
  );


  server.sendHeader(
    "Location",
    "/"
  );


  server.send(303);

}


// ============================================================
// SERIAL STATUS
// ============================================================

void printStatus() {

  Serial.println();

  Serial.println(
    "===== KYUURIQ STATUS ====="
  );


  Serial.print(
    "LED: "
  );


  Serial.println(
    ledState
      ? "ON"
      : "OFF"
  );


  Serial.print(
    "Brightness: "
  );


  Serial.print(
    brightness
  );


  Serial.println("%");


  Serial.print(
    "Mode: "
  );


  Serial.println(
    modeName(ledMode)
  );


  Serial.print(
    "Free heap: "
  );


  Serial.println(
    ESP.getFreeHeap()
  );


  Serial.print(
    "Uptime: "
  );


  Serial.print(
    millis() / 1000
  );


  Serial.println(
    " sec"
  );


  Serial.print(
    "WiFi IP: "
  );


  Serial.println(
    WiFi.localIP()
  );


  Serial.print(
    "WiFi RSSI: "
  );


  Serial.print(
    WiFi.RSSI()
  );


  Serial.println(
    " dBm"
  );


  Serial.println(
    "=========================="
  );

}


// ============================================================
// SERIAL COMMANDS
// ============================================================

void handleCommand(
  String cmd
) {

  cmd.trim();

  cmd.toLowerCase();


  // ==========================================================
  // ON
  // ==========================================================

  if (cmd == "on") {

    setLED(true);


    Serial.println(
      "LED: ON"
    );

  }


  // ==========================================================
  // OFF
  // ==========================================================

  else if (cmd == "off") {

    setLED(false);


    Serial.println(
      "LED: OFF"
    );

  }


  // ==========================================================
  // STATUS
  // ==========================================================

  else if (cmd == "status") {

    printStatus();

  }


  // ==========================================================
  // REBOOT
  // ==========================================================

  else if (cmd == "reboot") {

    Serial.println(
      "Rebooting..."
    );


    delay(500);


    ESP.restart();

  }


  // ==========================================================
  // HELP
  // ==========================================================

  else if (cmd == "help") {

    Serial.println();

    Serial.println(
      "KYUURIQ ESP v0.5.0"
    );


    Serial.println();

    Serial.println(
      "Commands:"
    );


    Serial.println(
      "  on"
    );


    Serial.println(
      "  off"
    );


    Serial.println(
      "  brightness 0-100"
    );


    Serial.println(
      "  mode solid"
    );


    Serial.println(
      "  mode blink"
    );


    Serial.println(
      "  mode fast"
    );


    Serial.println(
      "  mode slow"
    );


    Serial.println(
      "  mode pulse"
    );


    Serial.println(
      "  mode breathing"
    );


    Serial.println(
      "  status"
    );


    Serial.println(
      "  reboot"
    );


    Serial.println(
      "  help"
    );


    Serial.println();

  }


  // ==========================================================
  // BRIGHTNESS
  // ==========================================================

  else if (
    cmd.startsWith(
      "brightness "
    )
  ) {

    int value =
      cmd.substring(
        11
      ).toInt();


    value =
      constrain(
        value,
        0,
        100
      );


    setBrightness(
      value
    );


    Serial.print(
      "Brightness: "
    );


    Serial.print(
      value
    );


    Serial.println("%");

  }


  // ==========================================================
  // MODE
  // ==========================================================

  else if (
    cmd.startsWith(
      "mode "
    )
  ) {

    String value =
      cmd.substring(
        5
      );


    LEDMode newMode =
      modeFromString(
        value
      );


    setMode(
      newMode
    );


    Serial.print(
      "Mode: "
    );


    Serial.println(
      modeName(newMode)
    );

  }


  // ==========================================================
  // UNKNOWN
  // ==========================================================

  else if (
    cmd.length() > 0
  ) {

    Serial.print(
      "Unknown command: "
    );


    Serial.println(
      cmd
    );

  }

}


// ============================================================
// OTA
// ============================================================

void setupOTA() {

  ArduinoOTA.setHostname(
    "KYUURIQ-ESP"
  );


  ArduinoOTA.onStart(
    []() {

      Serial.println();

      Serial.println(
        "OTA: START"
      );

    }
  );


  ArduinoOTA.onEnd(
    []() {

      Serial.println();

      Serial.println(
        "OTA: END"
      );

    }
  );


  ArduinoOTA.onProgress(
    [](unsigned int progress,
       unsigned int total) {

      Serial.printf(
        "OTA: %u%%\r",
        (progress * 100) / total
      );

    }
  );


  ArduinoOTA.onError(
    [](ota_error_t error) {

      Serial.printf(
        "OTA ERROR: %u\n",
        error
      );

    }
  );


  ArduinoOTA.begin();


  Serial.println(
    "OTA: READY"
  );

}


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(
    115200
  );


  delay(500);


  Serial.println();

  Serial.println(
    "================================"
  );


  Serial.println(
    "        KYUURIQ ESP v0.5.0"
  );


  Serial.println(
    "        ESP32-WROOM-32"
  );


  Serial.println(
    "================================"
  );


  Serial.println();


  // ==========================================================
  // SETTINGS
  // ==========================================================

  loadSettings();


  // ==========================================================
  // PWM
  // ==========================================================

  setupPWM();


  // ==========================================================
  // LED
  // ==========================================================

  resetEffect();

  applyLED();


  // ==========================================================
  // WIFI
  // ==========================================================

  Serial.print(
    "Connecting to WiFi"
  );


  WiFi.mode(
    WIFI_STA
  );


  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );


  int attempts = 0;


  while (
    WiFi.status() != WL_CONNECTED &&
    attempts < 30
  ) {

    delay(500);

    Serial.print(".");

    attempts++;

  }


  Serial.println();


  // ==========================================================
  // WIFI CONNECTED
  // ==========================================================

  if (
    WiFi.status() == WL_CONNECTED
  ) {

    Serial.println(
      "WiFi: CONNECTED"
    );


    Serial.print(
      "IP: "
    );


    Serial.println(
      WiFi.localIP()
    );


    Serial.print(
      "RSSI: "
    );


    Serial.print(
      WiFi.RSSI()
    );


    Serial.println(
      " dBm"
    );


    // ========================================================
    // WEB SERVER
    // ========================================================

    server.on(
      "/",
      handleRoot
    );


    server.on(
      "/on",
      handleOn
    );


    server.on(
      "/off",
      handleOff
    );


    server.on(
      "/brightness",
      handleBrightness
    );


    server.on(
      "/mode",
      handleMode
    );


    server.begin();


    Serial.println(
      "Web server: STARTED"
    );


    // ========================================================
    // OTA
    // ========================================================

    setupOTA();


    Serial.println();


    Serial.println(
      "Open the IP address above."
    );

  }


  // ==========================================================
  // WIFI FAILED
  // ==========================================================

  else {

    Serial.println(
      "WiFi: FAILED"
    );


    Serial.println(
      "Check secrets.h"
    );

  }


  Serial.println();


  Serial.println(
    "Type 'help' for commands."
  );


  Serial.println();


#ifdef KQ_MENU
  mainMenuRun();
#endif

}


// ============================================================
// LOOP
// ============================================================

void loop() {

  if (
    WiFi.status() == WL_CONNECTED
  ) {

    server.handleClient();

    ArduinoOTA.handle();

  }


  // ==========================================================
  // LED EFFECT ENGINE
  // ==========================================================

  updateEffect();


  // ==========================================================
  // SERIAL
  // ==========================================================

  if (Serial.available()) {

    command =
      Serial.readStringUntil(
        '\n'
      );


    handleCommand(
      command
    );

  }

}