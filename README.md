# KYUURIQ ESP

Custom firmware for ESP32-WROOM-32.

KYUURIQ ESP is a small experimental firmware focused on learning ESP32 development, GPIO control, Wi-Fi and web-based device management.

## Features

### v0.2

* ESP32-WROOM-32 support
* LED control
* Serial command interface
* Wi-Fi connection
* Web control panel
* Blink modes
* System information
* Free heap monitoring
* Uptime
* ESP32 reboot command

## Planned

* GPIO control
* PWM
* Brightness control
* Improved web interface
* Persistent settings
* OTA firmware updates

## Hardware

Tested on:

* ESP32-WROOM-32
* USB Type-C ESP32 development board
* CH340 USB-UART

## Installation

### 1. Install Arduino IDE

Install Arduino IDE and the ESP32 board package.

Select:

`ESP32 Dev Module`

Select the COM port belonging to your ESP32 board.

### 2. Configure Wi-Fi

Copy:

`firmware/secrets.example.h`

to:

`firmware/secrets.h`

Then edit `secrets.h`:

```cpp
#pragma once

#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

Do not commit `secrets.h` to Git.

It is intentionally excluded by `.gitignore`.

### 3. Upload

Open:

`firmware/KYUURIQ_ESP.ino`

Compile and upload it to the ESP32.

Open Serial Monitor at:

`115200 baud`

After connecting to Wi-Fi, the ESP32 will print its local IP address.

Open that IP address in a browser connected to the same network.

## Serial Commands

```text
on
off
blink
fast
slow
status
reboot
help
```

## Version

Current version:

**v0.2**

## Project status

Experimental / educational project.

KYUURIQ ESP is being developed incrementally while learning ESP32 firmware development.
