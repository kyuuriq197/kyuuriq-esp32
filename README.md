# KYUURIQ ESP

<p align="center">
  <b>Local ESP32 Control Platform</b><br>
  Локальная платформа управления ESP32 от Kyuuriq
</p>

<p align="center">
  <img src="https://img.shields.io/badge/version-0.4.2-brightgreen">
  <img src="https://img.shields.io/badge/platform-ESP32-blue">
  <img src="https://img.shields.io/badge/framework-Arduino-orange">
  <img src="https://img.shields.io/badge/control-Local-purple">
  <img src="https://img.shields.io/badge/status-Active-success">
</p>

---

## About / О проекте

**KYUURIQ ESP** — собственная прошивка и экспериментальная embedded-платформа для **ESP32-WROOM-32**.

**KYUURIQ ESP** is a custom firmware and experimental embedded platform for the **ESP32-WROOM-32**.

Проект создаётся для изучения embedded-разработки, микроконтроллеров, Wi-Fi, Web-интерфейсов, PWM, OTA и взаимодействия с аппаратными модулями.

The project is designed for learning embedded development, microcontrollers, Wi-Fi, Web interfaces, PWM, OTA and hardware modules.

Главная идея проекта — постепенно превратить обычную ESP32-плату в собственную локальную платформу управления.

The main idea is to gradually turn a standard ESP32 development board into a custom local control platform.

KYUURIQ ESP работает **локально** и не требует облачного сервера для управления устройством.

KYUURIQ ESP works **locally** and does not require a cloud server for device control.

---

## Features / Возможности

- Wi-Fi connectivity / Подключение к Wi-Fi
- Local Web interface / Локальная Web-панель
- Built-in LED control / Управление встроенным LED
- PWM brightness control / PWM-регулировка яркости
- 6 LED modes / 6 режимов LED
- Persistent settings / Сохранение настроек
- Serial console / Serial-консоль
- OTA firmware updates / OTA-обновление прошивки
- System information / Информация о системе
- Device reboot / Перезагрузка устройства
- Local-only operation / Полностью локальная работа
- No cloud dependency / Без зависимости от облачных сервисов

---

## LED Modes / Режимы LED

KYUURIQ ESP currently supports six LED modes.

KYUURIQ ESP поддерживает шесть режимов работы LED.

| Mode | Description |
|---|---|
| **Solid** | Constant light / Постоянное свечение |
| **Blink** | Normal blinking / Обычное мигание |
| **Fast Blink** | Fast blinking / Быстрое мигание |
| **Slow Blink** | Slow blinking / Медленное мигание |
| **Pulse** | Pulsing effect / Пульсация |
| **Breathing** | Smooth breathing effect / Плавное свечение |

---

## Hardware / Аппаратная часть

### Main board / Основная плата

- ESP32-WROOM-32
- USB Type-C
- CH340 USB-to-Serial
- Built-in LED
- GPIO 2 — onboard LED

### Current target

```text
ESP32-WROOM-32
GPIO 2 → Built-in LED
Wi-Fi  → Local Web Server
USB    → Serial / Initial flashing
Software / Программная часть

KYUURIQ ESP is currently built using the Arduino framework for ESP32.

KYUURIQ ESP использует Arduino framework для ESP32.

Main components
Arduino ESP32 Core
WiFi
WebServer
ArduinoOTA
Preferences
Serial Console
Used functionality
Wi-Fi
  ↓
ESP32 WebServer
  ↓
Local Web Interface
  ↓
LED / Brightness / Modes
Web Interface / Web-панель

После запуска и подключения к Wi-Fi ESP32 поднимает локальный Web-сервер.

After connecting to Wi-Fi, the ESP32 starts a local Web server.

Открой IP-адрес устройства в браузере:

http://ESP32-IP

Например:

http://192.168.1.123
Web UI provides / Web-панель позволяет
Turn LED ON / Включить LED
Turn LED OFF / Выключить LED
Change brightness / Изменить яркость
Select LED mode / Выбрать режим LED
View device information / Посмотреть информацию об устройстве
View RAM information / Посмотреть RAM
View uptime / Посмотреть время работы
View Wi-Fi RSSI / Посмотреть уровень Wi-Fi
Reboot ESP32 / Перезагрузить ESP32
Brightness / Яркость

Brightness is controlled using hardware PWM.

Яркость управляется аппаратным PWM.

Current range:

0%   → OFF
1–99% → Adjustable brightness
100% → Maximum brightness

The onboard LED is active-low, therefore the PWM logic is inverted.

Встроенный LED использует active-low логику, поэтому PWM управляется с инверсией.

Serial Console / Serial-консоль

KYUURIQ ESP also provides a command-line interface through Serial Monitor.

KYUURIQ ESP также предоставляет консоль управления через Serial Monitor.

Commands / Команды
on
off

brightness 0-100

mode solid
mode blink
mode fast
mode slow
mode pulse
mode breathing

status
reboot
help
Example / Пример
brightness 75

Устанавливает яркость LED на 75%.

Sets LED brightness to 75%.

Persistent Settings / Сохранение настроек

KYUURIQ ESP uses ESP32 Preferences storage.

Настройки сохраняются во встроенной энергонезависимой памяти ESP32.

Сохраняются параметры, необходимые для восстановления состояния устройства после перезагрузки.

The device can restore its configuration after reboot.

Current persistent settings include:

LED state
Brightness
LED mode
OTA

KYUURIQ ESP supports firmware updates over Wi-Fi using OTA (Over-The-Air).

KYUURIQ ESP поддерживает обновление прошивки по Wi-Fi через OTA (Over-The-Air).

После первоначальной прошивки через USB новые версии можно загружать через Arduino IDE по сети.

After the initial USB upload, new firmware versions can be uploaded through Arduino IDE over Wi-Fi.

OTA hostname
KYUURIQ-ESP

После запуска ESP32 устройство может появиться в Arduino IDE как сетевой порт.

After boot, the ESP32 can appear in Arduino IDE as a network port.

OTA Architecture / Архитектура OTA

Текущая схема обновления:

Developer
    │
    ▼
Arduino IDE
    │
    │ Wi-Fi / OTA
    ▼
ESP32
    │
    ▼
KYUURIQ ESP Firmware

GitHub используется для хранения исходного кода проекта.

GitHub is used to store the project source code.

Текущая версия OTA не скачивает прошивку автоматически с GitHub.

The current OTA implementation does not automatically download firmware from GitHub.

Automatic GitHub-based firmware updates are planned for a future version.

Installation / Установка
1. Install Arduino IDE

Установите Arduino IDE.

Install Arduino IDE.

2. Install ESP32 board support

Добавьте поддержку ESP32 в Arduino IDE.

Install the ESP32 board package and select the correct ESP32 board.

3. Configure Wi-Fi

Создайте файл:

secrets.h

Рядом с основным .ino файлом:

kq_esp32/
├── kq_esp32.ino
├── secrets.h
└── README.md

Пример структуры secrets.h:

#pragma once

#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"

Do not upload secrets.h with real credentials to GitHub.

Не публикуйте secrets.h с настоящими Wi-Fi данными в GitHub.

4. Connect ESP32

Подключите ESP32 к компьютеру через USB.

Connect the ESP32 to your computer using USB.

5. Upload firmware

Первую прошивку необходимо выполнить через USB.

The first firmware upload must be performed through USB.

После этого OTA может использоваться для следующих обновлений.

After that, OTA can be used for future updates.

Project Structure / Структура проекта
kyuuriq-esp32/
│
├── firmware/
│   └── kq_esp32/
│       ├── kq_esp32.ino
│       └── secrets.h
│
├── README.md
│
└── LICENSE

secrets.h является локальным конфигурационным файлом.

secrets.h is a local configuration file.

Он не должен содержать реальные credentials в публичном репозитории.

It should not expose real credentials in a public repository.

Local Architecture / Локальная архитектура

KYUURIQ ESP не требует внешнего сервера для управления.

KYUURIQ ESP does not require an external server for control.

                LOCAL NETWORK
                     │
        ┌────────────┴────────────┐
        │                         │
     Browser                    ESP32
        │                         │
        │ HTTP                    │
        └─────────────────────────┘
                  │
                  ▼
           KYUURIQ ESP
                  │
        ┌─────────┼─────────┐
        ▼         ▼         ▼
       LED       PWM      Modes

Вся основная логика управления выполняется непосредственно на ESP32.

All main control logic runs directly on the ESP32.

Security / Безопасность

KYUURIQ ESP is designed primarily as a local embedded project.

KYUURIQ ESP в первую очередь является локальным embedded-проектом.

Не публикуйте:

Wi-Fi passwords
API keys
Tokens
Private credentials
Other sensitive configuration

Не добавляйте реальные секреты в GitHub.

Never commit real credentials to the repository.

Roadmap
Completed / Готово
 ESP32 base firmware
 Wi-Fi connectivity
 Local Web Server
 Local Web UI
 Built-in LED control
 ON / OFF control
 PWM brightness
 6 LED modes
 Persistent settings
 Serial console
 Device status
 System information
 OTA firmware updates
 Reboot control
In Progress / В разработке
 Better Web UI
 More LED effects
 Hardware module support
 External LED control
 Sensor support
 Better system monitoring
Planned / Планируется
 GPIO control
 External LEDs
 Buttons
 Sensors
 OLED displays
 SD card support
 Additional ESP32 modules
 Web-based configuration
 Firmware version management
 Automatic OTA from GitHub Releases
 Automatic update checking
 Plugin-like hardware modules
 More advanced Web UI
 Device logs
 More system information
 Multiple ESP32 device support
Version / Версия

Current version:

v0.4.2

Development status:

ACTIVE
Future Vision / Будущее проекта

KYUURIQ ESP is intended to grow from a simple LED firmware into a modular ESP32 platform.

Цель проекта — постепенно превратить простую прошивку LED в полноценную модульную платформу для ESP32.

Possible future architecture:

                 KYUURIQ ESP
                      │
        ┌─────────────┼─────────────┐
        │             │             │
       WEB          SERIAL          OTA
        │             │             │
        └─────────────┼─────────────┘
                      │
                 ESP32 CORE
                      │
       ┌──────────────┼──────────────┐
       │              │              │
      LED          SENSORS        MODULES
       │              │              │
       └──────────────┼──────────────┘
                      │
                  HARDWARE

Проект будет развиваться постепенно, начиная с базовых функций ESP32 и переходя к более сложным аппаратным модулям.

The project will evolve gradually, starting with basic ESP32 functionality and moving toward more advanced hardware modules.

Author / Автор

Kyuuriq

KYUURIQ ESP — personal embedded development project.

KYUURIQ ESP — личный проект по изучению embedded-разработки.

License

This project is currently under development.

License information will be added as the project matures.
