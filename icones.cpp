#include "icones.h"

// =====================================================
// ÍCONE DA LUZ
// =====================================================

void iconeLuz(Adafruit_SSD1306 &display, int x, int y, bool ligado) {

  if (!ligado)
    return;

  display.drawCircle(x, y, 5, SSD1306_WHITE);

  display.drawLine(x - 3, y + 5, x + 3, y + 5, SSD1306_WHITE);
  display.drawLine(x - 2, y + 7, x + 2, y + 7, SSD1306_WHITE);

  display.drawLine(x, y - 8, x, y - 11, SSD1306_WHITE);
  display.drawLine(x - 7, y - 6, x - 9, y - 8, SSD1306_WHITE);
  display.drawLine(x + 7, y - 6, x + 9, y - 8, SSD1306_WHITE);
  display.drawLine(x - 8, y, x - 11, y, SSD1306_WHITE);
  display.drawLine(x + 8, y, x + 11, y, SSD1306_WHITE);
}

// =====================================================
// ÍCONE DO EXAUSTOR
// =====================================================

void iconeExaustor(Adafruit_SSD1306 &display, int x, int y, bool ligado) {

  if (!ligado)
    return;

  display.drawCircle(x, y, 2, SSD1306_WHITE);

  display.drawLine(x, y - 2, x, y - 9, SSD1306_WHITE);
  display.drawLine(x + 2, y, x + 9, y - 3, SSD1306_WHITE);
  display.drawLine(x, y + 2, x + 4, y + 8, SSD1306_WHITE);
  display.drawLine(x - 2, y, x - 9, y + 3, SSD1306_WHITE);
}

// =====================================================
// ÍCONE DO SOPRADOR
// =====================================================

void iconeSoprador(Adafruit_SSD1306 &display, int x, int y, bool ligado) {

  if (!ligado)
    return;

  display.drawRect(
    x - 8,
    y - 5,
    8,
    10,
    SSD1306_WHITE
  );

  display.drawLine(x, y - 3, x + 7, y - 3, SSD1306_WHITE);
  display.drawLine(x, y + 3, x + 7, y + 3, SSD1306_WHITE);

  display.drawLine(x + 9, y - 5, x + 13, y - 5, SSD1306_WHITE);
  display.drawLine(x + 9, y,     x + 14, y,     SSD1306_WHITE);
  display.drawLine(x + 9, y + 5, x + 13, y + 5, SSD1306_WHITE);
}

// =====================================================
// ÍCONE DO LED
// =====================================================

void iconeLED(Adafruit_SSD1306 &display, int x, int y, bool ligado) {

  if (!ligado)
    return;

  display.drawCircle(x, y, 5, SSD1306_WHITE);

  display.drawLine(x, y - 9, x, y - 6, SSD1306_WHITE);
  display.drawLine(x, y + 6, x, y + 9, SSD1306_WHITE);
  display.drawLine(x - 9, y, x - 6, y, SSD1306_WHITE);
  display.drawLine(x + 6, y, x + 9, y, SSD1306_WHITE);
}
