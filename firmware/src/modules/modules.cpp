#include "module.h"

void wifiOpen();
void bleOpen();
void irOpen();
void gpioOpen();
void toolsOpen();
void settingsOpen();
void infoOpen();

const Module MODULES[] = {
    {"WiFi", wifiOpen},
    {"BLE", bleOpen},
    {"IR", irOpen},
    {"GPIO / LED", gpioOpen},
    {"Tools", toolsOpen},
    {"Settings", settingsOpen},
    {"About", infoOpen},
};

const int MODULE_COUNT = sizeof(MODULES) / sizeof(MODULES[0]);
