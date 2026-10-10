#pragma once

#include <Arduino.h>

enum class Btn { NONE, UP, DOWN, OK, BACK };

void inputSetup();
Btn inputPoll();
