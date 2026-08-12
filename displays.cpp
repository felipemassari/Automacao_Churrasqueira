#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "estado.h"
#include "menu.h"
#include "icones.h"
#include "displays.h"

Adafruit_SSD1306 display1(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

Adafruit_SSD1306 display2(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// =====================================================
// DISPLAY 1
// =====================================================

void atualizarDisplay1() {

  display1.clearDisplay();

  // -------------------------
  // Título
  // -------------------------

  display1.setTextColor(SSD1306_WHITE);
  display1.setTextSize(1);

  display1.setCursor(0, 0);
  display1.println("CHURRASQUEIRA");

  // -------------------------
  // Temperatura
  // -------------------------

  display1.setTextSize(3);
  display1.setCursor(15, 16);

  if (temperatura >= 0 && temperatura < 1000)
    display1.print((int)temperatura);
  else
    display1.print("---");

  display1.setTextSize(1);
  display1.setCursor(91, 28);
  display1.print("C");

  // -------------------------
  // Linha separadora
  // -------------------------

  display1.drawLine(0, 47, 127, 47, SSD1306_WHITE);

  // -------------------------
  // Ícones
  // -------------------------

  iconeLuz(display1, 18, 56, luz);
  iconeExaustor(display1, 55, 56, exaustor);
  iconeSoprador(display1, 91, 56, soprador);
  iconeLED(display1, 119, 56, led);

  display1.display();
}

// =====================================================
// DISPLAY 2
// =====================================================

void atualizarDisplay2() {

  display2.clearDisplay();

  display2.setTextColor(SSD1306_WHITE);

  // -------------------------
  // Nome do item
  // -------------------------

  display2.setTextSize(2);
  display2.setCursor(0, 0);
  display2.println(menu[menuAtual]);

  // -------------------------
  // Itens normais
  // -------------------------

  if (menuAtual < 4) {

    bool estado = estadoMenuAtual();

    display2.setTextSize(3);
    display2.setCursor(28, 25);

    if (estado)
      display2.println("ON");
    else
      display2.println("OFF");
  }

  // -------------------------
  // Config
  // -------------------------

  else {

    display2.setTextSize(1);

    display2.setCursor(15, 32);
    display2.println("Pressione para");

    display2.setCursor(15, 45);
    display2.println("entrar");
  }

  // -------------------------
  // Indicadores de navegação
  // -------------------------

  display2.setTextSize(1);

  display2.setCursor(0, 56);
  display2.print("<");

  display2.setCursor(120, 56);
  display2.print(">");

  display2.display();
}

// =====================================================
// TELA PROVISÓRIA DE CONFIG
// =====================================================

void mostrarConfig() {

  display2.clearDisplay();

  display2.setTextColor(SSD1306_WHITE);

  display2.setTextSize(2);
  display2.setCursor(20, 5);
  display2.println("CONFIG");

  display2.setTextSize(1);
  display2.setCursor(15, 35);
  display2.println("Tela provisoria");

  display2.setCursor(15, 48);
  display2.println("sera criada depois");

  display2.display();
}

// =====================================================
// INICIALIZAÇÃO DOS OLEDs
// =====================================================

void iniciarDisplays() {

  if (!display1.begin(
        SSD1306_SWITCHCAPVCC,
        OLED1_ADDRESS
      )) {

    Serial.println("ERRO OLED 1 - 0x3D");

  } else {

    Serial.println("OLED 1 OK - 0x3D");
  }

  if (!display2.begin(
        SSD1306_SWITCHCAPVCC,
        OLED2_ADDRESS
      )) {

    Serial.println("ERRO OLED 2 - 0x3C");

  } else {

    Serial.println("OLED 2 OK - 0x3C");
  }
}
