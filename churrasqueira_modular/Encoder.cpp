#include "Encoder.h"

#include <Arduino.h>

#include "Config.h"
#include "State.h"
#include "Display.h"

bool estadoMenuAtual() {

  if (menuAtual == 0) return luz;
  if (menuAtual == 1) return exaustor;
  if (menuAtual == 2) return soprador;

  return false;
}

void inicializarEncoder() {

  pinMode(ENC_S1, INPUT_PULLUP);
  pinMode(ENC_S2, INPUT_PULLUP);
  pinMode(ENC_KEY, INPUT_PULLUP);

  ultimoS1 = digitalRead(ENC_S1);
  ultimoEstadoBotao = digitalRead(ENC_KEY);
}

// ============================================================
// BOTÃO
// ============================================================

void verificarBotao() {

  int estado = digitalRead(ENC_KEY);

  if (estado == LOW && ultimoEstadoBotao == HIGH) {

    if (millis() - ultimoClique > DEBOUNCE) {

      ultimoClique = millis();

      if (editandoOpcao) {

        if (menuAtual == 0) {
          luz = estadoTemporario;
          Serial.print("Luz confirmado: ");
          Serial.println(luz ? "LIGADO" : "DESLIGADO");
        }

        else if (menuAtual == 1) {
          exaustor = estadoTemporario;
          Serial.print("Exaustor confirmado: ");
          Serial.println(exaustor ? "LIGADO" : "DESLIGADO");
        }

        else if (menuAtual == 2) {
          soprador = estadoTemporario;
          Serial.print("Soprador confirmado: ");
          Serial.println(soprador ? "LIGADO" : "DESLIGADO");
        }

        editandoOpcao = false;

        atualizarDisplay2Opcao();
        atualizarDisplay1();
      }

      else {

        if (menuAtual == 3) {

          Serial.println("CONFIG selecionado");
          atualizarDisplay2Opcao();
        }

        else {

          estadoTemporario = estadoMenuAtual();
          editandoOpcao = true;

          Serial.print("Entrando em: ");
          Serial.println(menu[menuAtual]);

          atualizarDisplay2Edicao();
        }
      }
    }
  }

  ultimoEstadoBotao = estado;
}

// ============================================================
// ROTAÇÃO
// ============================================================

void verificarEncoder() {

  int s1 = digitalRead(ENC_S1);

  if (s1 != ultimoS1) {

    if (s1 == LOW) {

      int s2 = digitalRead(ENC_S2);

      if (editandoOpcao) {

        estadoTemporario = !estadoTemporario;

        Serial.print("Selecao: ");
        Serial.println(estadoTemporario ? "LIGADO" : "DESLIGADO");

        atualizarDisplay2Edicao();
      }

      else {

        if (s2 != s1) {

          menuAtual++;

          if (menuAtual >= MENU_TOTAL)
            menuAtual = 0;
        }

        else {

          menuAtual--;

          if (menuAtual < 0)
            menuAtual = MENU_TOTAL - 1;
        }

        Serial.print("Menu: ");
        Serial.println(menu[menuAtual]);

        atualizarDisplay2Opcao();
      }
    }
    retornoStatus = millis();
    ultimoS1 = s1;
  }
    if (millis() - retornoStatus > 10000){
      retornoStatus = millis();
      editandoOpcao = false;
      atualizarDisplay2Status();   
    }
}
