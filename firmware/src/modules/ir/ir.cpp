#include "../../core/display.h"
#include "../../core/menu.h"

void irSendCodes() {
  displayClear();
  displayTitle("IR Send");
  displayLine(0, "TODO", false);
}

void irReceive() {
  displayClear();
  displayTitle("IR Receiver");
  displayLine(0, "TODO", false);
}

void irRemote() {
  displayClear();
  displayTitle("IR Remote");
  displayLine(0, "TODO", false);
}

void irOpen() {
  static const MenuItem items[] = {
      {"Send Code", irSendCodes},
      {"Receiver", irReceive},
      {"Remote", irRemote},
  };
  menuRun({"IR", items, 3});
}
