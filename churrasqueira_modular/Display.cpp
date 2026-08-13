#include "Display.h"

#include <stdio.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

#include "Config.h"
#include "State.h"
#include "Icons.h"

extern Adafruit_SSD1306 display1;
extern Adafruit_SSD1306 display2;

// ============================================================
// DESENHAR ÍCONE 20x20
// ============================================================

void desenharIcone20(
  Adafruit_SSD1306 &display,
  const uint8_t icone[20][20],
  int x, int y
) {
  for (int linha = 0; linha < 20; linha++) {
    for (int coluna = 0; coluna < 20; coluna++) {
      if (icone[linha][coluna] == 1) {
        display.drawPixel(x + coluna, y + linha, SSD1306_WHITE);
      }
    }
  }
}

// ============================================================
// DESENHAR ÍCONE 15x15
// ============================================================

void desenharIcone15(
  Adafruit_SSD1306 &display,
  const uint8_t icone[15][15],
  int x, int y
) {
  for (int linha = 0; linha < 15; linha++) {
    for (int coluna = 0; coluna < 15; coluna++) {
      if (icone[linha][coluna] == 1) {
        display.drawPixel(x + coluna, y + linha, SSD1306_WHITE);
      }
    }
  }
}

// ============================================================
// DISPLAY 1
// ============================================================

void atualizarDisplay1() {

  display1.clearDisplay();
  display1.setTextColor(SSD1306_WHITE);

  desenharIcone15(display1, ICON_TEMPERATURA,
                  POS_TEMP_ICON_X, POS_TEMP_ICON_Y);

  display1.setFont(&FreeMonoBold9pt7b);
  display1.setCursor(POS_TEMP_DHT_X, POS_TEMP_DHT_Y);
  display1.print(tempDHT);
  display1.print("C");

  desenharIcone15(display1, ICON_UMIDADE,
                  POS_UMID_ICON_X, POS_UMID_ICON_Y);

  display1.setFont(&FreeMonoBold9pt7b);
  display1.setCursor(POS_UMID_DHT_X, POS_UMID_DHT_Y);
  display1.print(umidDHT);
  display1.print("%");

  display1.setFont(&FreeMonoBold12pt7b);
  display1.setCursor(POS_BBQ_X, POS_BBQ_Y);
  display1.print("BBQ");

  display1.setFont(&FreeSansBold18pt7b);

  char tempTexto[10];
  sprintf(tempTexto, "%d", (int)temperatura);

  display1.setCursor(POS_TEMP_BBQ_X, POS_TEMP_BBQ_Y);
  display1.print(tempTexto);

  display1.setFont(&FreeSansBold12pt7b);
  display1.print("C");

  if (soprador) {
            desenharIcone20(display1, ICON_SOPRADOR,
                            POS_SOPRADOR_X, POS_SOPRADOR_Y);
  }
  if (luz) {
  desenharIcone20(display1, ICON_LUZ,
                  POS_LUZ_X, POS_LUZ_Y);
  }
  if (exaustor) {
  desenharIcone20(display1, ICON_EXAUSTOR,
                  POS_EXAUSTOR_X, POS_EXAUSTOR_Y);
  }

  display1.display();

}

// ============================================================
// DISPLAY 2 - STATUS
// ============================================================

void atualizarDisplay2Status() {

  display2.clearDisplay();
  display2.setTextColor(SSD1306_WHITE);

  display2.setFont(&FreeMono12pt7b);
  display2.setCursor(13, 15);
  display2.print("STATUS");

  display2.setFont(&FreeMonoBold9pt7b);

  display2.setCursor(1, 32);
  display2.print("Soprador");
  display2.setCursor(96, 32);
  display2.print(soprador ? "ON" : "OFF");

  display2.setCursor(1, 48);
  display2.print("Luz");
  display2.setCursor(96, 48);
  display2.print(luz ? "ON" : "OFF");

  display2.setCursor(1, 63);
  display2.print("Exaustor");
  display2.setCursor(96, 63);
  display2.print(exaustor ? "ON" : "OFF");

  display2.display();
}

// ============================================================
// DISPLAY 2 - MENU
// ============================================================

void atualizarDisplay2Opcao() {

  display2.clearDisplay();
  display2.setTextColor(SSD1306_WHITE);

  if (menuAtual == 3) {

    display2.setFont(&FreeMono12pt7b);
    display2.setCursor(30, 20);
    display2.print("CONFIG");

    display2.setFont(NULL);
    display2.setTextSize(1);

    display2.setCursor(18, 42);
    display2.print("Configuracao");

    display2.setCursor(18, 54);
    display2.print("sera feita depois");

    display2.display();

    return;
  }

  display2.setFont(&FreeSansBold12pt7b);

  display2.setCursor(10, 35);
  display2.print("Acionar");

  display2.setCursor(20, 63);
  display2.print(menu[menuAtual]);

  display2.display();

}

// ============================================================
// DISPLAY 2 - EDIÇÃO
// ============================================================

void atualizarDisplay2Edicao() {

  display2.clearDisplay();
  display2.setTextColor(SSD1306_WHITE);

  display2.setFont(&FreeMonoBold12pt7b);
  display2.setCursor(2, 15);
  display2.print(menu[menuAtual]);

  display2.setFont(&FreeMono12pt7b);
  display2.setCursor(1, 48);

  if (estadoTemporario)
    display2.print("LIGADO");
  else
    display2.print("DESLIGADO");

  display2.display();
}
