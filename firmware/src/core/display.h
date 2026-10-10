#pragma once

#include <Arduino.h>

void displaySetup();
void displayClear();
void displayTitle(const String& title);
void displayLine(int row, const String& text, bool selected);
void displayStatus(const String& left, const String& right);
void displaySplash(const String& version);
