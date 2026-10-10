#pragma once

#include <Arduino.h>

struct MenuItem {
  const char* label;
  void (*run)();
};

struct Menu {
  const char* title;
  const MenuItem* items;
  int count;
};

void menuRun(const Menu& menu);
