#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

#define LED_PIN 2

WebServer server(80);

bool ledState = false;
bool blinking = false;

unsigned long previousMillis = 0;
unsigned long blinkInterval = 500;

String command = "";

void setLED(bool state) {
  ledState = state;
  digitalWrite(LED_PIN, state ? LOW : HIGH);
}

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>KYUURIQ ESP</title>
<style>
body {
  font-family: Arial;
  text-align: center;
  background: #111;
  color: white;
  padding: 30px;
}
button {
  font-size: 20px;
  padding: 15px 25px;
  margin: 8px;
  border-radius: 10px;
  border: none;
}
.card {
  max-width: 500px;
  margin: auto;
  padding: 25px;
  background: #222;
  border-radius: 18px;
}
</style>
</head>

<body>
<div class="card">

<h1>KYUURIQ ESP</h1>
<p>ESP32-WROOM-32</p>

<hr>

<h2>LED</h2>

<p>
<a href="/on"><button>ON</button></a>
<a href="/off"><button>OFF</button></a>
</p>

<p>
<a href="/blink"><button>BLINK</button></a>
<a href="/fast"><button>FAST</button></a>
<a href="/slow"><button>SLOW</button></a>
</p>

<hr>

<h2>System</h2>

<p>LED: %LED%</p>
<p>Mode: %MODE%</p>
<p>Free RAM: %RAM% bytes</p>
<p>Uptime: %UPTIME% sec</p>

<hr>

<a href="/reboot">
<button>REBOOT ESP32</button>
</a>

</div>
</body>
</html>
)rawliteral";

  html.replace("%LED%", ledState ? "ON" : "OFF");

  if (blinking)
    html.replace("%MODE%", "BLINK");
  else
    html.replace("%MODE%", "STATIC");

  html.replace("%RAM%", String(ESP.getFreeHeap()));
  html.replace("%UPTIME%", String(millis() / 1000));

  return html;
}

void handleRoot() {
  server.send(200, "text/html", makePage());
}

void handleOn() {
  blinking = false;
  setLED(true);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  blinking = false;
  setLED(false);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBlink() {
  blinking = true;
  blinkInterval = 500;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleFast() {
  blinking = true;
  blinkInterval = 100;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSlow() {
  blinking = true;
  blinkInterval = 1000;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleReboot() {
  server.send(200, "text/html", "<h1>Rebooting...</h1>");
  delay(500);
  ESP.restart();
}

void printStatus() {
  Serial.println();
  Serial.println("===== KYUURIQ STATUS =====");

  Serial.print("LED: ");
  Serial.println(ledState ? "ON" : "OFF");

  Serial.print("Blinking: ");
  Serial.println(blinking ? "YES" : "NO");

  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());

  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" sec");

  Serial.print("WiFi IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("==========================");
}

void handleCommand(String cmd) {

  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "on") {
    blinking = false;
    setLED(true);
    Serial.println("LED: ON");
  }

  else if (cmd == "off") {
    blinking = false;
    setLED(false);
    Serial.println("LED: OFF");
  }

  else if (cmd == "blink") {
    blinking = true;
    blinkInterval = 500;
    Serial.println("Mode: BLINK");
  }

  else if (cmd == "fast") {
    blinking = true;
    blinkInterval = 100;
    Serial.println("Mode: FAST");
  }

  else if (cmd == "slow") {
    blinking = true;
    blinkInterval = 1000;
    Serial.println("Mode: SLOW");
  }

  else if (cmd == "status") {
    printStatus();
  }

  else if (cmd == "reboot") {
    Serial.println("Rebooting...");
    delay(500);
    ESP.restart();
  }

  else if (cmd == "help") {
    Serial.println();
    Serial.println("Commands:");
    Serial.println("  on");
    Serial.println("  off");
    Serial.println("  blink");
    Serial.println("  fast");
    Serial.println("  slow");
    Serial.println("  status");
    Serial.println("  reboot");
    Serial.println("  help");
    Serial.println();
  }

  else if (cmd.length() > 0) {
    Serial.print("Unknown command: ");
    Serial.println(cmd);
  }
}

void setup() {

  pinMode(LED_PIN, OUTPUT);

  // Active-low LED
  digitalWrite(LED_PIN, HIGH);

  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("==============================");
  Serial.println("       KYUURIQ ESP v0.2");
  Serial.println("       ESP32-WROOM-32");
  Serial.println("==============================");

  Serial.println();
  Serial.print("Connecting to WiFi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WiFi: CONNECTED");

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.on("/blink", handleBlink);
    server.on("/fast", handleFast);
    server.on("/slow", handleSlow);
    server.on("/reboot", handleReboot);

    server.begin();

    Serial.println("Web server: STARTED");
    Serial.println();
    Serial.println("Open the IP address above in your browser.");

  } else {

    Serial.println("WiFi: FAILED");
    Serial.println("Check SSID and password.");
  }

  Serial.println();
  Serial.println("Type 'help' for commands.");
  Serial.println();
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  if (blinking) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= blinkInterval) {

      previousMillis = currentMillis;

      setLED(!ledState);
    }
  }

  if (Serial.available()) {

    command = Serial.readStringUntil('\n');

    handleCommand(command);
  }
}
