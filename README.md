# KYUURIQ ESP

> Локальная ESP32-платформа управления от Kyuuriq.  
> Local ESP32 control platform by Kyuuriq.

---

## 🇷🇺 Русский

**KYUURIQ ESP** — собственная прошивка для ESP32-WROOM-32.

Проект объединяет локальную Web-панель, управление встроенным LED, PWM, LED-эффекты, сохранение настроек и OTA-прошивку.

### Текущая версия

**v0.4.2**

### Возможности

- Wi-Fi
- Локальная Web-панель
- Управление встроенным LED
- Регулировка яркости 0–100%
- Режимы LED:
  - Solid — постоянное свечение
  - Blink — мигание
  - Fast Blink — быстрое мигание
  - Slow Blink — медленное мигание
  - Pulse — пульсация
  - Breathing — плавное свечение
- Сохранение настроек
- OTA-обновление прошивки
- Управление через Serial Monitor
- Информация о системе
- Локальное управление без облака

### LED

Встроенный LED использует:

GPIO 2

Яркость:

0–100%

### Web-панель

После подключения ESP32 к Wi-Fi открой IP-адрес устройства в браузере:

http://ESP32-IP

Например:

http://192.168.1.123

Через Web-панель доступны:

- включение / выключение LED
- регулировка яркости
- выбор режима
- информация об устройстве
- перезагрузка

### Serial-команды

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

### OTA

После первой прошивки через USB ESP32 можно обновлять по Wi-Fi через Arduino IDE.

OTA hostname:

KYUURIQ-ESP

GitHub используется для хранения исходного кода и документации. Сам ESP32 работает локально и не зависит от GitHub.

### Hardware

ESP32-WROOM-32  
USB Type-C  
CH340

### Структура проекта

kyuuriq-esp32/  
├── kq_esp32.ino  
├── secrets.h  
└── README.md

`secrets.h` содержит данные Wi-Fi и не должен публиковаться в GitHub.

### Roadmap

- [x] Wi-Fi
- [x] Web-панель
- [x] Управление LED
- [x] PWM / яркость
- [x] LED-режимы
- [x] Сохранение настроек
- [x] OTA
- [x] Serial-команды
- [ ] Управление внешними LED
- [ ] Датчики
- [ ] Дополнительные модули
- [ ] Автоматический OTA через GitHub Releases

---

## 🇬🇧 English

**KYUURIQ ESP** is a custom firmware for the ESP32-WROOM-32.

The project combines a local Web UI, built-in LED control, PWM brightness, LED effects, persistent settings and OTA firmware updates.

### Current Version

**v0.4.2**

### Features

- Wi-Fi
- Local Web UI
- Built-in LED control
- 0–100% brightness control
- LED modes:
  - Solid
  - Blink
  - Fast Blink
  - Slow Blink
  - Pulse
  - Breathing
- Persistent settings
- OTA firmware updates
- Serial Monitor commands
- System information
- Local-only control

### LED

Built-in LED:

GPIO 2

Brightness:

0–100%

### Web UI

After connecting to Wi-Fi, open the ESP32 IP address in your browser:

http://ESP32-IP

Example:

http://192.168.1.123

The Web UI provides:

- LED ON / OFF
- Brightness control
- Mode selection
- Device information
- Reboot

### Serial Commands

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

### OTA

After the first USB firmware upload, the ESP32 can be updated over Wi-Fi using Arduino IDE.

OTA hostname:

KYUURIQ-ESP

GitHub is used for source code and documentation. The ESP32 itself operates locally and does not depend on GitHub.

### Hardware

ESP32-WROOM-32  
USB Type-C  
CH340

### Project Structure

kyuuriq-esp32/  
├── kq_esp32.ino  
├── secrets.h  
└── README.md

`secrets.h` contains local Wi-Fi credentials and must not be published.

### Roadmap

- [x] Wi-Fi
- [x] Web UI
- [x] LED control
- [x] PWM / brightness
- [x] LED modes
- [x] Persistent settings
- [x] OTA
- [x] Serial commands
- [ ] External LED control
- [ ] Sensors
- [ ] Additional hardware modules
- [ ] Automatic OTA via GitHub Releases

---

**KYUURIQ ESP · Local control · v0.4.2**
