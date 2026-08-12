#ifndef ICONES_H
#define ICONES_H

#include <Adafruit_SSD1306.h>

// Os ícones são desenhados diretamente com as primitivas
// do Adafruit_GFX. Isso mantém o desenho leve e fácil de
// alterar depois.

// Desenha o ícone da lâmpada somente quando ligado.
void iconeLuz(Adafruit_SSD1306 &display, int x, int y, bool ligado);

// Desenha o ícone do exaustor somente quando ligado.
void iconeExaustor(Adafruit_SSD1306 &display, int x, int y, bool ligado);

// Desenha o ícone do soprador somente quando ligado.
void iconeSoprador(Adafruit_SSD1306 &display, int x, int y, bool ligado);

// Desenha o ícone do LED somente quando ligado.
void iconeLED(Adafruit_SSD1306 &display, int x, int y, bool ligado);

#endif
