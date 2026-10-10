#include "mainmenu.h"
#include "menu.h"
#include "../modules/module.h"

void mainMenuRun() {
  static MenuItem items[16];
  static bool built = false;
  if (!built) {
    for (int i = 0; i < MODULE_COUNT && i < 16; i++) {
      items[i] = {MODULES[i].name, MODULES[i].open};
    }
    built = true;
  }
  menuRun({"KQ ESP", items, MODULE_COUNT});
}
