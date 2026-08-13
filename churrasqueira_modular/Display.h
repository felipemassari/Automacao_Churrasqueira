#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>

void atualizarDisplay1();
void atualizarDisplay2Status();
void atualizarDisplay2Opcao();
void atualizarDisplay2Edicao();

void desenharIcone20(
  Adafruit_SSD1306 &display,
  const uint8_t icone[20][20],
  int x, int y
);

void desenharIcone15(
  Adafruit_SSD1306 &display,
  const uint8_t icone[15][15],
  int x, int y
);

#endif
