#include "menu.h"
#include "estado.h"

const char* menu[] = {
  "Luz",
  "Exaustor",
  "Soprador",
  "LED",
  "Config"
};

const int MENU_TOTAL = 5;

int menuAtual = 0;

bool estadoMenuAtual() {

  if (menuAtual == 0)
    return luz;

  if (menuAtual == 1)
    return exaustor;

  if (menuAtual == 2)
    return soprador;

  if (menuAtual == 3)
    return led;

  return false;
}
