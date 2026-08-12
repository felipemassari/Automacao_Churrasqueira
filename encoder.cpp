#include <Arduino.h>

#include "config.h"
#include "estado.h"
#include "menu.h"
#include "displays.h"

static int ultimoS1 = HIGH;
static unsigned long ultimoClique = 0;

void iniciarEncoder() {

  pinMode(ENC_S1, INPUT_PULLUP);
  pinMode(ENC_S2, INPUT_PULLUP);
  pinMode(ENC_KEY, INPUT_PULLUP);

  ultimoS1 = digitalRead(ENC_S1);
}

// =====================================================
// BOTÃO DO ENCODER
// =====================================================
//
// O clique é tratado somente quando o botão passa de
// solto (HIGH) para pressionado (LOW).
//
// Isso evita que manter o botão pressionado gere vários
// comandos consecutivos.
// =====================================================

void verificarBotao() {

  int estado = digitalRead(ENC_KEY);

  static int ultimoEstadoBotao = HIGH;

  if (estado == LOW && ultimoEstadoBotao == HIGH) {

    if (millis() - ultimoClique > DEBOUNCE) {

      ultimoClique = millis();

      if (menuAtual == 0) {

        luz = !luz;

        Serial.print("Luz: ");
        Serial.println(luz ? "ON" : "OFF");
      }

      else if (menuAtual == 1) {

        exaustor = !exaustor;

        Serial.print("Exaustor: ");
        Serial.println(exaustor ? "ON" : "OFF");
      }

      else if (menuAtual == 2) {

        soprador = !soprador;

        Serial.print("Soprador: ");
        Serial.println(soprador ? "ON" : "OFF");
      }

      else if (menuAtual == 3) {

        led = !led;

        Serial.print("LED: ");
        Serial.println(led ? "ON" : "OFF");
      }

      else if (menuAtual == 4) {

        Serial.println("CONFIG selecionado");

        mostrarConfig();
        delay(1000);
      }

      atualizarDisplay1();
      atualizarDisplay2();
    }
  }

  ultimoEstadoBotao = estado;
}

// =====================================================
// ROTAÇÃO DO ENCODER
// =====================================================

void verificarEncoder() {

  int s1 = digitalRead(ENC_S1);

  if (s1 != ultimoS1) {

    if (s1 == LOW) {

      int s2 = digitalRead(ENC_S2);

      if (s2 != s1) {

        // Sentido horário.
        menuAtual++;

        if (menuAtual >= MENU_TOTAL)
          menuAtual = 0;

      } else {

        // Sentido anti-horário.
        menuAtual--;

        if (menuAtual < 0)
          menuAtual = MENU_TOTAL - 1;
      }

      Serial.print("Menu: ");
      Serial.println(menu[menuAtual]);

      atualizarDisplay2();
    }

    ultimoS1 = s1;
  }
}
