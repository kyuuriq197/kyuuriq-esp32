#include "menu.h"
#include "display.h"
#include "input.h"

namespace {
void render(const Menu& menu, int selected) {
  displayClear();
  displayTitle(menu.title);
  for (int i = 0; i < menu.count; i++) {
    displayLine(i, menu.items[i].label, i == selected);
  }
}
}

void menuRun(const Menu& menu) {
  if (menu.count == 0) return;

  int selected = 0;
  render(menu, selected);

  while (true) {
    switch (inputPoll()) {
      case Btn::UP:
        selected = (selected + menu.count - 1) % menu.count;
        render(menu, selected);
        break;
      case Btn::DOWN:
        selected = (selected + 1) % menu.count;
        render(menu, selected);
        break;
      case Btn::OK:
        if (menu.items[selected].run) {
          menu.items[selected].run();
          render(menu, selected);
        }
        break;
      case Btn::BACK:
        return;
      default:
        break;
    }
    delay(10);
  }
}
