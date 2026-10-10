#pragma once

struct Module {
  const char* name;
  void (*open)();
};

extern const Module MODULES[];
extern const int MODULE_COUNT;
