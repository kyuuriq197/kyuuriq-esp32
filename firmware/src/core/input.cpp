#include "input.h"
#include "../config.h"

namespace {
const int PINS[] = {BTN_UP_PIN, BTN_DOWN_PIN, BTN_OK_PIN, BTN_BACK_PIN};
const Btn CODES[] = {Btn::UP, Btn::DOWN, Btn::OK, Btn::BACK};
constexpr int PIN_COUNT = 4;

unsigned long lastChange[4] = {0, 0, 0, 0};
bool stable[4] = {false, false, false, false};
bool lastRaw[4] = {false, false, false, false};
unsigned long lastRepeat = 0;
}

void inputSetup() {
  for (int i = 0; i < PIN_COUNT; i++) {
    pinMode(PINS[i], INPUT_PULLUP);
  }
}

Btn inputPoll() {
  unsigned long now = millis();

  for (int i = 0; i < PIN_COUNT; i++) {
    bool raw = digitalRead(PINS[i]) == LOW;
    if (raw != lastRaw[i]) {
      lastRaw[i] = raw;
      lastChange[i] = now;
    }
    if (now - lastChange[i] >= 25) stable[i] = raw;
    if (stable[i] && now - lastRepeat > 350) {
      lastRepeat = now;
      return CODES[i];
    }
  }
  return Btn::NONE;
}
