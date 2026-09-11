#include "Encoder.h"

#include <Arduino.h>

#include "Config.h"
#include "State.h"
#include "Display.h"
#include "Connectivity.h"

bool estadoMenuAtual() {

  if (menuAtual == 0) return luz;
  if (menuAtual == 1) return exaustor;
  if (menuAtual == 2) return soprador;

  return false;
}

void inicializarEncoder() {

  pinMode(ENC_S1, INPUT);
  pinMode(ENC_S2, INPUT);
  pinMode(ENC_KEY, INPUT_PULLUP);

  ultimoS1 = digitalRead(ENC_S1);
  ultimoS2 = digitalRead(ENC_S2);
  ultimoEstadoBotao = digitalRead(ENC_KEY);
}

/// ============================================================
// BOTÃO
// ============================================================
void verificarBotao() {

  int estado = digitalRead(ENC_KEY);

  if (estado == LOW && ultimoEstadoBotao == HIGH) {

    if (millis() - ultimoClique > DEBOUNCE) {

      ultimoClique = millis();
      retornoStatus = millis();

      // ========================================================
      // 1. ESTÁ EDITANDO
      // ========================================================
      if (editandoOpcao) {


        // ------------------------------------------------------
        // 1A. EDITANDO MENU CONFIG
        // ------------------------------------------------------
        if (dentroConfig) {

          // VOLTAR
          if (menuConfigAtual == 5) {

            editandoOpcao = false;
            dentroConfig = false;
            aguardandoStatus = false;

            menuAtual = 0;

            atualizarDisplay2Status();
          }

          // CONFIRMAR CONFIGURAÇÃO
          else {

            if (menuConfigAtual == 0) {
              modoAutomatico = estadoConfigTemp;
            }

            else if (menuConfigAtual == 1) {
              tempoTela = valorConfigTemp;
            }

            else if (menuConfigAtual == 2) {
              setTempExaustor = valorConfigTemp;
            }

            else if (menuConfigAtual == 3) {
              setTempSoprador = valorConfigTemp;
            }

            else if (menuConfigAtual == 4) {
              ledMureta = estadoConfigTemp;
              digitalWrite(RELE_LED, ledMureta ? LOW : HIGH);
            }

            editandoOpcao = false;

            aguardandoStatus = true;

            Serial.print("Configuracao confirmada: ");
            Serial.println(menuConfig[menuConfigAtual]);

            atualizarDisplay2Config();
          }
        }


        // ------------------------------------------------------
        // 1B. EDITANDO MENU PRINCIPAL
        // ------------------------------------------------------
        else {

          if (menuAtual == 0) {

            luz = estadoTemporario;

            digitalWrite(RELE_LUZ, luz ? LOW : HIGH);

            Serial.print("Luz confirmado: ");
            Serial.println(luz ? "LIGADO" : "DESLIGADO");
            publicarMQTT();
          }

          else if (menuAtual == 1) {

            exaustor = estadoTemporario;

            digitalWrite(RELE_EXAUSTOR, exaustor ? LOW : HIGH);

            Serial.print("Exaustor confirmado: ");
            Serial.println(exaustor ? "LIGADO" : "DESLIGADO");
            publicarMQTT();
          }

          else if (menuAtual == 2) {

            soprador = estadoTemporario;

            digitalWrite(RELE_SOPRADOR, soprador ? LOW : HIGH);
            publicarMQTT();

            Serial.print("Soprador confirmado: ");
            Serial.println(soprador ? "LIGADO" : "DESLIGADO");
          }

          editandoOpcao = false;

          atualizarDisplay2Opcao();
          atualizarDisplay1();
        }
      }


      // ========================================================
      // 2. NÃO ESTÁ EDITANDO
      // ========================================================
      else {
        // ======================================================
        // 2A. ESTÁ DENTRO DO MENU CONFIG
        // ======================================================
        if (dentroConfig) {

          // ----------------------------------------------------
          // VOLTAR
          // ----------------------------------------------------
          if (menuConfigAtual == 5) {

            dentroConfig = false;
            editandoOpcao = false;
            aguardandoStatus = false;

            menuAtual = 0;

            atualizarDisplay2Status();
          }
          // ----------------------------------------------------
          // ENTRAR NA EDIÇÃO DA CONFIGURAÇÃO
          // ----------------------------------------------------
          else {

            // Modo Automático
            if (menuConfigAtual == 0) {

              estadoConfigTemp = modoAutomatico;
            }

            // Tempo de Tela
            else if (menuConfigAtual == 1) {

              valorConfigTemp = tempoTela;
            }

            // Temperatura Exaustor
            else if (menuConfigAtual == 2) {

              valorConfigTemp = setTempExaustor;
            }

            // Temperatura Soprador
            else if (menuConfigAtual == 3) {

              valorConfigTemp = setTempSoprador;
            }

            // LED Mureta
            else if (menuConfigAtual == 4) {

              estadoConfigTemp = ledMureta;
            }

            editandoOpcao = true;

            Serial.print("Editando config: ");
            Serial.println(menuConfig[menuConfigAtual]);

            atualizarDisplay2ConfigEdicao();
          }
        }


        // ======================================================
        // 2B. MENU PRINCIPAL → ENTRAR NO CONFIG
        // ======================================================
        else if (menuAtual == 3) {

          dentroConfig = true;
          menuConfigAtual = 0;

          Serial.println("CONFIG selecionado");

          atualizarDisplay2Config();
        }


        // ======================================================
        // 2C. MENU PRINCIPAL → LUZ / EXAUSTOR / SOPRADOR
        // ======================================================
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
// ROTAÇÃO DO ENCODER
// ============================================================
void verificarEncoder() {

  int s1 = digitalRead(ENC_S1);
  // ==========================================================
  // MUDANÇA NO S1
  // ==========================================================
  if (s1 != ultimoS1) {

    // Só processa HIGH -> LOW
    if (ultimoS1 == HIGH && s1 == LOW) {

      delay(2);

      // Confirma S1 LOW
      if (digitalRead(ENC_S1) == LOW) {

        if (millis() - ultimoMovimento > DEBOUNCE_ENCODER) {

          ultimoMovimento = millis();
          retornoStatus = millis();

          int s2 = digitalRead(ENC_S2);

          int direcao;

          if (s2 == HIGH) {
            direcao = -1;
          }
          else {
            direcao = 1;
          }


          // ==================================================
          // EDITANDO
          // ==================================================
          if (editandoOpcao) {
            // ------------------------------------------------
            // CONFIG
            // ------------------------------------------------
            if (dentroConfig) {
              alterarConfig(direcao);
              Serial.print("Giro: ");              

              if (direcao > 0)
                Serial.println("HORARIO");
              else
                Serial.println("ANTI-HORARIO");
                Serial.print("Valor temporario: ");
                Serial.println(valorConfigTemp);
                
                atualizarDisplay2ConfigEdicao();
            }

            // ------------------------------------------------
            // LUZ / EXAUSTOR / SOPRADOR
            // ------------------------------------------------
            else {

              estadoTemporario = !estadoTemporario;

              Serial.print("Selecao: ");
              Serial.println(estadoTemporario ? "LIGADO" : "DESLIGADO");
              atualizarDisplay2Edicao();
            }
          }

          // ==================================================
          // NÃO ESTÁ EDITANDO
          // ==================================================
          else {
            // ------------------------------------------------
            // MENU CONFIG
            // ------------------------------------------------
            if (dentroConfig) {

              if (direcao > 0) {

                menuConfigAtual++;

                if (menuConfigAtual >= MENU_CONFIG_TOTAL)
                  menuConfigAtual = 0;
              }

              else {

                menuConfigAtual--;

                if (menuConfigAtual < 0)
                  menuConfigAtual = MENU_CONFIG_TOTAL - 1;
              }

              Serial.print("Config: ");
              Serial.println(menuConfig[menuConfigAtual]);
              atualizarDisplay2Config();
              
            }


            // ------------------------------------------------
            // MENU PRINCIPAL
            // ------------------------------------------------
            else {

              if (direcao > 0) {

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

          
        }
      }
    }

    // Atualiza S1
    ultimoS1 = s1;
  }
  // ==========================================================
  // RETORNO 3 SEGUNDOS APÓS CONFIRMAR CONFIG
  // ==========================================================
  if (aguardandoStatus &&
      millis() - retornoStatus >= 3000) {

    aguardandoStatus = false;

    editandoOpcao = false;
    dentroConfig = false;

    menuAtual = 0;

    atualizarDisplay2Status();
  }
}

  // ============================================================
  // ALTERA PARAMETROS DA CONFIGURACAO
  // ============================================================
void alterarConfig(int direcao) {

  // Modo automático
  if (menuConfigAtual == 0) {
    estadoConfigTemp = !estadoConfigTemp;
  }

  // Tempo de tela
  else if (menuConfigAtual == 1) {
    valorConfigTemp += direcao;

    if (valorConfigTemp < 3)
      valorConfigTemp = 3;

    if (valorConfigTemp > 60)
      valorConfigTemp = 60;
  }

  // Temperatura exaustor
  else if (menuConfigAtual == 2) {
    valorConfigTemp += direcao * 5;

    if (valorConfigTemp < 20)
      valorConfigTemp = 20;

    if (valorConfigTemp > 150)
      valorConfigTemp = 150;
  }

  // Temperatura soprador
  else if (menuConfigAtual == 3) {
    valorConfigTemp += direcao * 5;

    if (valorConfigTemp < 20)
      valorConfigTemp = 20;

    if (valorConfigTemp > 150)
      valorConfigTemp = 150;
  }

  // LED mureta
  else if (menuConfigAtual == 4) {
    estadoConfigTemp = !estadoConfigTemp;
  }
}

  // ============================================================
  // CONTROLA CURRASQUEIRA SOZINHO COM VALORES DO PRESET
  // ============================================================
void controleAutomatico() {

  if (!modoAutomatico)
    return;

  // EXAUSTOR
  if (temperatura >= setTempExaustor) {
    exaustor = true;
    digitalWrite(RELE_EXAUSTOR, LOW);
  }
  else {
    exaustor = false;
    digitalWrite(RELE_EXAUSTOR, HIGH);
  }

  // SOPRADOR
  if (temperatura >= setTempSoprador) {
    soprador = true;
    digitalWrite(RELE_SOPRADOR, LOW);
  }
  else {
    soprador = false;
    digitalWrite(RELE_SOPRADOR, HIGH);
  }
}

void tempoDeTela() {

  unsigned long tempoDecorrido = millis() - retornoStatus;
  unsigned long tempoLimite = tempoTela * 1000UL;

  /*Serial.print("Decorrido: ");
  Serial.print(tempoDecorrido);
  Serial.print(" ms | Limite: ");
  Serial.print(tempoLimite);
  Serial.print(" ms | aguardando: ");
  Serial.println(aguardandoStatus);*/

  if (!aguardandoStatus && tempoDecorrido > tempoLimite) {
    //Serial.println(">>> VOLTANDO PARA STATUS <<<");
    editandoOpcao = false;
    dentroConfig = false;

    menuAtual = 0;

    atualizarDisplay2Status();
  }
}
