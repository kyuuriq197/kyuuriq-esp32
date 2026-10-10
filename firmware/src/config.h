#pragma once

#define KQ_VERSION "0.5.2"

#define LED_PIN 2
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

#define BTN_UP_PIN 5
#define BTN_DOWN_PIN 6
#define BTN_OK_PIN 7
#define BTN_BACK_PIN 8

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 240

#if __has_include(<Adafruit_ST7789.h>)
#define KQ_MENU 1
#endif
