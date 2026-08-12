#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display1;
extern Adafruit_SSD1306 display2;

void iniciarDisplays();
void atualizarDisplay1();
void atualizarDisplay2();
void mostrarConfig();

#endif
