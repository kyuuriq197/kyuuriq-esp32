<!-- Логотип KQ ESP -->

<p align="center">
      <img width="1003" height="249" alt="mylogo" src="https://github.com/user-attachments/assets/35f20653-ec79-4180-b400-0c9f8d662b7b" />
</p>

<!-- Плашки / Бейджи -->

<p align="center">
      <a href="https://github.com/kyuuriq197/kyuuriq-esp32"><img src="https://img.shields.io/badge/KQ%20ESP-v0.5.2-65d6a6?style=flat" alt="KQ ESP Badge"></a>
      <a href="https://www.arduino.cc/"><img src="https://img.shields.io/badge/Arduino-ESP32-00979D?style=flat&logo=arduino" alt="Arduino Badge"></a>
      <a href="https://www.espressif.com/en/products/socs/esp32"><img src="https://img.shields.io/badge/ESP32-WROOM--32-E7352C?style=flat" alt="ESP32 Badge"></a>
      <a href="LICENSE"><img src="https://img.shields.io/badge/Control-Local-6f42c1?style=flat" alt="Local Control Badge"></a>
      <a href="LICENSE"><img src="https://img.shields.io/badge/Status-Active-2ea44f?style=flat" alt="Status Badge"></a>
</p>

<!-- Текст -->

<h2 align="center">About</h2>

<p align="center">
      KQ ESP is a custom local control platform for the ESP32-WROOM-32.<br>
      KQ ESP is built as a personal embedded development project focused on learning,<br>
      experimentation and creating a custom ESP32 firmware platform.
</p>

<p align="center">
      The project currently features Wi-Fi, a local Web interface, PWM brightness control,<br>
      LED effects, persistent settings, Serial Console and OTA firmware updates.
</p>

<!-- Возможности -->

<h2 align="center">Features</h2>

<p align="center">
      Wi-Fi · Local Web UI · Web Menu · Improv Wi-Fi Setup · AP Setup Portal · PWM · LED Modes · Preferences · Serial Console · OTA
</p>

<p align="center">
      Modular source: <code>firmware/src/</code><br>
      core (menu · display · input) + modules (WiFi · BLE · IR · GPIO · Tools)
</p>

<p align="center">
      <b>6 LED Modes:</b><br>
      Solid · Blink · Fast Blink · Slow Blink · Pulse · Breathing
</p>

<!-- Локальная работа -->

<h2 align="center">Local Control</h2>

<p align="center">
      KQ ESP is designed to operate locally.<br>
      No cloud server is required to control the device.
</p>

<p align="center">
      ESP32 → Wi-Fi → Local Web Interface
</p>

<!-- Flasher -->

<h2 align="center">Flasher</h2>

<p align="center">
      KQ ESP includes a browser-based USB firmware flasher for ESP32-WROOM-32.<br>
      The flasher uses Web Serial and works directly from the browser.
</p>

<p align="center">
      <a href="https://kyuuriq197.github.io/kyuuriq-esp32/flasher/">
            <img src="https://img.shields.io/badge/Open-KQ%20ESP%20Flasher-65d6a6?style=flat" alt="Open KQ ESP Flasher">
      </a>
</p>

<p align="center">
      Connect ESP32 → Erase Flash → Flash KQ ESP → Reset
</p>

<p align="center">
      Right after install the browser asks for your Wi-Fi (<b>Improv</b>) and sends it over USB.<br>
      No prompt? Connect to the <code>KQ-ESP-Setup</code> access point and open http://192.168.4.1
</p>

<p align="center">
      Step-by-step guide for regular users: <a href="INSTALL.md"><b>INSTALL.md</b></a>
</p>

<!-- OTA -->

<h2 align="center">OTA</h2>

<p align="center">
      KQ ESP supports Over-The-Air firmware updates through Wi-Fi.
</p>

<p align="center">
      After the initial USB installation, firmware can be uploaded<br>
      directly to the ESP32 through Arduino IDE over the local network.
</p>

<!-- Дисклеймер -->

<h2 align="center">Disclaimer</h2>

<p align="center">
      KQ ESP is an educational and experimental embedded project.<br>
      Use the firmware and connected hardware responsibly and only on systems and devices you own or are authorized to test.
</p>

<!-- Безопасность -->

<h2 align="center">Security</h2>

<p align="center">
      Never publish Wi-Fi passwords, API keys, tokens or other private credentials.
</p>

<p align="center">
      Local configuration files such as <code>secrets.h</code> should not contain<br>
      real credentials when uploaded to a public repository.
</p>

<!-- Лицензия -->

<h2 align="center">License</h2>

<p align="center">
      KQ ESP is an open-source personal embedded project.
</p>

<p align="center">
      License information will be added as the project develops.
</p>

<!-- Credits -->

<h2 align="center">Credits</h2>

<p align="center">
      <a href="https://github.com/kyuuriq197"><img src="https://img.shields.io/badge/Main%20Development-Kyuuriq-65d6a6?style=flat&logo=github" alt="Kyuuriq Badge"></a>
      <a href="https://www.arduino.cc/"><img src="https://img.shields.io/badge/Framework-Arduino-00979D?style=flat&logo=arduino" alt="Arduino Badge"></a>
      <a href="https://www.espressif.com/"><img src="https://img.shields.io/badge/Hardware-Espressif-E7352C?style=flat" alt="Espressif Badge"></a>
</p>

<!-- Roadmap -->

<h2 align="center">Roadmap</h2>

<p align="center">
      <b>Current</b><br>
      Wi-Fi · Web UI · PWM · LED Modes · Preferences · Serial · OTA · USB Flasher · Modular Menu · Improv Wi-Fi
</p>

<p align="center">
      <b>Planned</b><br>
      Sensors · External LEDs · OLED · SD Card · GPIO Control · More Hardware Modules · Automatic OTA
</p>

<!-- Версия -->

<h2 align="center">Version</h2>

<p align="center">
      <b>v0.5.2</b>
</p>

<p align="center">
      <b>KQ ESP</b><br>
      Local Control · Embedded · ESP32
</p>
