#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>


// =====================================================
// I2C
// =====================================================
#define SDA_PIN 3
#define SCL_PIN 1


// =====================================================
// OLED
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define OLED1_ADDRESS 0x3D
#define OLED2_ADDRESS 0x3C

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
// MAX6675
// =====================================================
#define MAX_SCK 0
#define MAX_CS  2
#define MAX_SO  21

MAX6675 termopar(MAX_SCK, MAX_CS, MAX_SO);


// =====================================================
// ENCODER
// =====================================================
#define ENC_S1  10
#define ENC_S2  8
#define ENC_KEY 6


// =====================================================
// POSIÇÕES - DISPLAY 1
// =====================================================
// X aumenta para a direita
// Y aumenta para baixo
// OLED = 128 x 64
// -----------------------------------------------------
// ÍCONE TEMPERATURA DHT
// -----------------------------------------------------
#define POS_TEMP_ICON_X  2
#define POS_TEMP_ICON_Y  0

// -----------------------------------------------------
// VALOR TEMPERATURA DHT
// -----------------------------------------------------
#define POS_TEMP_DHT_X   20
#define POS_TEMP_DHT_Y   14

// -----------------------------------------------------
// ÍCONE UMIDADE
// -----------------------------------------------------
#define POS_UMID_ICON_X  70
#define POS_UMID_ICON_Y  0

// -----------------------------------------------------
// VALOR UMIDADE
// -----------------------------------------------------
#define POS_UMID_DHT_X   88
#define POS_UMID_DHT_Y   14

// -----------------------------------------------------
// BBQ
// -----------------------------------------------------
#define POS_BBQ_X        1
#define POS_BBQ_Y        35

// -----------------------------------------------------
// TEMPERATURA DA CHURRASQUEIRA
// -----------------------------------------------------
#define POS_TEMP_BBQ_X   45
#define POS_TEMP_BBQ_Y   40

// -----------------------------------------------------
// ÍCONES DOS EQUIPAMENTOS
// -----------------------------------------------------
#define POS_SOPRADOR_X   5
#define POS_SOPRADOR_Y   44

#define POS_LUZ_X        54
#define POS_LUZ_Y        45

#define POS_EXAUSTOR_X   103
#define POS_EXAUSTOR_Y   45

// =====================================================
// VALORES DHT - SIMULAÇÃO
// =====================================================
int tempDHT = 25;
int umidDHT = 60;

// =====================================================
// ESTADOS DOS EQUIPAMENTOS
// =====================================================
bool luz = false;
bool exaustor = false;
bool soprador = false;

// =====================================================
// MENU
// =====================================================
const char* menu[] = {
  "Luz",
  "Exaustor",
  "Soprador",
  "Config"
};

const int MENU_TOTAL = 4;
int menuAtual = 0;

// =====================================================
// CONTROLE DE EDIÇÃO
// =====================================================
bool editandoOpcao = false;

// =====================================================
// ENCODER
// =====================================================
bool estadoTemporario = false;

int ultimoS1 = HIGH;

int ultimoEstadoBotao = HIGH;

unsigned long ultimoClique = 0;

const unsigned long DEBOUNCE = 250;

// =====================================================
// TEMPERATURA DA CHURRASQUEIRA
// =====================================================
float temperatura = 0;

unsigned long ultimaLeituraTemp = 0;

// =====================================================
// ÍCONE 20 x 20
// =====================================================
const uint8_t ICON_SOPRADOR[20][20] = {

  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,1,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0}

};


const uint8_t ICON_LUZ[20][20] = {

  {0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
  {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
  {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0}

};


const uint8_t ICON_EXAUSTOR[20][20] = {

  {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0},
  {0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
  {1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0},
  {1,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0},
  {1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1,0},
  {1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1},
  {1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1},
  {0,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,0,1},
  {0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1},
  {0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0}

};


// =====================================================
// ÍCONE TEMPERATURA 15 x 15
// =====================================================

const uint8_t ICON_TEMPERATURA[15][15] = {

  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,1,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,0,0,0,0,0}

};


// =====================================================
// ÍCONE UMIDADE 15 x 15
// =====================================================

const uint8_t ICON_UMIDADE[15][15] = {

  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0,0,1,0,0,0,0,0},
  {0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,1,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,1,1,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,0,1,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0}

};


// =====================================================
// FUNÇÃO PARA DESENHAR ÍCONE 20x20
// =====================================================
void desenharIcone20(
  Adafruit_SSD1306 &display,
  const uint8_t icone[20][20],
  int x,
  int y
) {

  for (int linha = 0; linha < 20; linha++) {

    for (int coluna = 0; coluna < 20; coluna++) {

      if (icone[linha][coluna] == 1) {
        display.drawPixel(
          x + coluna,
          y + linha,
          SSD1306_WHITE
        );

      }

    }

  }

}


// =====================================================
// FUNÇÃO PARA DESENHAR ÍCONE 15x15
// =====================================================
void desenharIcone15(
  Adafruit_SSD1306 &display,
  const uint8_t icone[15][15],
  int x,
  int y
) {

  for (int linha = 0; linha < 15; linha++) {

    for (int coluna = 0; coluna < 15; coluna++) {

      if (icone[linha][coluna] == 1) {
        display.drawPixel(
          x + coluna,
          y + linha,
          SSD1306_WHITE
        );

      }

    }

  }

}


// =====================================================
// RETORNA O ESTADO DO ITEM SELECIONADO
// =====================================================
bool estadoMenuAtual() {

  if (menuAtual == 0)
    return luz;

  if (menuAtual == 1)
    return exaustor;

  if (menuAtual == 2)
    return soprador;

  return false;
}


// =====================================================
// DISPLAY 1
// =====================================================
// Layout:
// [ ícone temp ] 25°C    [ ícone gota ] 60%
//
//             BBQ  213°C
//
// [soprador]       [luz]       [exaustor]
// =====================================================

void atualizarDisplay1() {

  display1.clearDisplay();

  display1.setTextColor(SSD1306_WHITE);


  // ===================================================
  // TEMPERATURA DHT
  // ===================================================
  desenharIcone15(
    display1,
    ICON_TEMPERATURA,
    POS_TEMP_ICON_X,
    POS_TEMP_ICON_Y
  );

  display1.setFont(&FreeMonoBold9pt7b);

  display1.setCursor(
    POS_TEMP_DHT_X,
    POS_TEMP_DHT_Y
  );

  display1.print(tempDHT);
  display1.print("C");

  // ===================================================
  // UMIDADE DHT
  // ===================================================
  desenharIcone15(
    display1,
    ICON_UMIDADE,
    POS_UMID_ICON_X,
    POS_UMID_ICON_Y
  );

  display1.setFont(&FreeMonoBold9pt7b);

  display1.setCursor(
    POS_UMID_DHT_X,
    POS_UMID_DHT_Y
  );

  display1.print(umidDHT);
  display1.print("%");

  // ===================================================
  // BBQ
  // ===================================================
  display1.setFont(&FreeMonoBold12pt7b);

  display1.setCursor(
    POS_BBQ_X,
    POS_BBQ_Y
  );

  display1.print("BBQ");

  // ===================================================
  // TEMPERATURA DA CHURRASQUEIRA
  // ===================================================
  display1.setFont(&FreeSansBold18pt7b);

  char tempTexto[10];

  sprintf(tempTexto,"%d",(int)temperatura);

  display1.setCursor(
    POS_TEMP_BBQ_X,
    POS_TEMP_BBQ_Y
  );

  display1.print(tempTexto);
  display1.setFont(&FreeSansBold12pt7b);
  display1.print("C");

  // ===================================================
  // ÍCONES DOS EQUIPAMENTOS
  // ===================================================
  desenharIcone20(
    display1,
    ICON_SOPRADOR,
    POS_SOPRADOR_X,
    POS_SOPRADOR_Y
  );
  desenharIcone20(
    display1,
    ICON_LUZ,
    POS_LUZ_X,
    POS_LUZ_Y
  );
  desenharIcone20(
    display1,
    ICON_EXAUSTOR,
    POS_EXAUSTOR_X,
    POS_EXAUSTOR_Y
  );

  // ===================================================
  // ATUALIZA OLED
  // ===================================================
  display1.display();
}

// =====================================================
// DISPLAY 2 - STATUS
// =====================================================
void atualizarDisplay2Status() {

  display2.clearDisplay();

  display2.setTextColor(SSD1306_WHITE);


  // ===================================================
  // TÍTULO
  // ===================================================
  display2.setFont(&FreeMono12pt7b);

  display2.setCursor(13, 15);

  display2.print("STATUS");

  // ===================================================
  // SOPRADOR
  // ===================================================
  display2.setFont(&FreeMonoBold9pt7b);

  display2.setCursor(1, 32);

  display2.print("Soprador");

  display2.setCursor(96, 32);

  display2.print(
    soprador ? "ON" : "OFF"
  );

  // ===================================================
  // LUZ
  // ===================================================
  display2.setCursor(1, 48);

  display2.print("Luz");

  display2.setCursor(96, 48);

  display2.print(
    luz ? "ON" : "OFF"
  );

  // ===================================================
  // EXAUSTOR
  // ===================================================
  display2.setCursor(1, 63);

  display2.print("Exaustor");

  display2.setCursor(96, 63);

  display2.print(
    exaustor ? "ON" : "OFF"
  );

  display2.display();
}

// =====================================================
// DISPLAY 2 - MENU
// =====================================================
void atualizarDisplay2Opcao() {

  display2.clearDisplay();

  display2.setTextColor(SSD1306_WHITE);

  // ===================================================
  // CONFIG
  // ===================================================
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


  // ===================================================
  // "MUDAR OPCAO"
  // ===================================================
  display2.setFont(&FreeSansBold12pt7b);

  display2.setCursor(10, 40);

  display2.print("Mudar");

  display2.setCursor(5, 63);

  display2.print(
    menu[menuAtual]
  );


  display2.display();
}


// =====================================================
// DISPLAY 2 - EDIÇÃO
// =====================================================
void atualizarDisplay2Edicao() {

  display2.clearDisplay();

  display2.setTextColor(SSD1306_WHITE);

  display2.setFont(&FreeMonoBold12pt7b);

  display2.setCursor(2, 15);

  display2.print(
    menu[menuAtual]
  );


  // ===================================================
  // ESTADO TEMPORÁRIO
  // ===================================================

  display2.setFont(&FreeMono12pt7b);

  display2.setCursor(1, 48);

  if (estadoTemporario) { display2.print("LIGADO");  }

  else {    display2.print("DESLIGADO");  }

  display2.display();
}


// =====================================================
// BOTÃO DO ENCODER ETECTA APENAS A TRANSIÇÃO HIGH -> LOW
// =====================================================
void verificarBotao() {

  int estado = digitalRead(ENC_KEY);

  if (
    estado == LOW &&
    ultimoEstadoBotao == HIGH
  ) {

    if (
      millis() - ultimoClique >
      DEBOUNCE
    ) {

      ultimoClique = millis();

        // ===============================================
        // LUZ
        // ===============================================
      if (editandoOpcao) {
        if (menuAtual == 0) {

          luz = estadoTemporario;
          Serial.print("Luz confirmado: ");
          Serial.println(
            luz ? "LIGADO" : "DESLIGADO"
          );

        }
        // ===============================================
        // EXAUSTOR
        // ===============================================
        else if (menuAtual == 1) {

          exaustor = estadoTemporario;
          Serial.print("Exaustor confirmado: ");
          Serial.println(
            exaustor ? "LIGADO" : "DESLIGADO"
          );

        }
        // ===============================================
        // SOPRADOR
        // ===============================================
        else if (menuAtual == 2) {

          soprador = estadoTemporario;
          Serial.print("Soprador confirmado: ");
          Serial.println(
            soprador ? "LIGADO" : "DESLIGADO"
          );

        }
        // ===============================================
        // SAI DA EDIÇÃO
        // ===============================================

        editandoOpcao = false;
        // Volta para a tela "Mudar..."
        atualizarDisplay2Opcao();
        // Atualiza os ícones
        atualizarDisplay1();

      }

      else {
        // ===============================================
        // CONFIG
        // ===============================================
        if (menuAtual == 3) {

          Serial.println(
            "CONFIG selecionado"
          );
          atualizarDisplay2Opcao();

        }
        // ===============================================
        // ENTRA NA OPÇÃO
        // ===============================================

        else {

          estadoTemporario =
            estadoMenuAtual();

          editandoOpcao = true;

          Serial.print(
            "Entrando em: "
          );
          Serial.println(
            menu[menuAtual]
          );

          atualizarDisplay2Edicao();

        }

      }

    }

  }
  ultimoEstadoBotao = estado;
}


// =====================================================
// ENCODER - ROTAÇÃO
// =====================================================
void verificarEncoder() {

  int s1 = digitalRead(ENC_S1);


  // ===================================================
  // DETECTA MUDANÇA NO S1, PROCESSA SOMENTE QUANDO S1 VAI PARA LOW
  // ===================================================

  if (s1 != ultimoS1) {

    if (s1 == LOW) {

      int s2 = digitalRead(ENC_S2);

      // ===============================================
      // ESTÁ EDITANDO UMA OPÇÃO
      // ===============================================

      if (editandoOpcao) {

        // Girar o encoder simplesmente
        // alterna entre LIGADO e DESLIGADO.

        estadoTemporario =
          !estadoTemporario;

        Serial.print(
          "Selecao: "
        );

        Serial.println(
          estadoTemporario
          ? "LIGADO"
          : "DESLIGADO"
        );


        atualizarDisplay2Edicao();

      }


      // ===============================================
      // ESTÁ NAVEGANDO PELO MENU
      // ===============================================

      else {
        if (s2 != s1) {
          menuAtual++;

          if (
            menuAtual >= MENU_TOTAL
          ) {

            menuAtual = 0;

          }

        }
        else {
          menuAtual--;

          if (menuAtual < 0) {

            menuAtual =
              MENU_TOTAL - 1;

          }

        }


        Serial.print(
          "Menu: "
        );

        Serial.println(
          menu[menuAtual]
        );


        atualizarDisplay2Opcao();

      }

    }

    // Guarda o estado do S1
    ultimoS1 = s1;

  }

}


// =====================================================
// LEITURA DA TEMPERATURA
// =====================================================
void lerTemperatura() {

  if (
    millis() - ultimaLeituraTemp < 1000
  ) {

    return;

  }

  ultimaLeituraTemp =
    millis();

  temperatura =
    termopar.readCelsius();

  Serial.print(
    "Temperatura BBQ: "
  );

  Serial.print(
    temperatura
  );

  Serial.println(
    " C"
  );

  atualizarDisplay1();
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("   CHURRASQUEIRA");
  Serial.println("==============================");

  // ===================================================
  // I2C
  // ===================================================
  Wire.begin(
    SDA_PIN,
    SCL_PIN
  );

  delay(100);


  // ===================================================
  // SCANNER I2C
  // ===================================================
  byte encontrados = 0;

  Serial.println(
    "Procurando dispositivos I2C..."
  );

  for (
    byte endereco = 1;
    endereco < 127;
    endereco++
  ) {

    Wire.beginTransmission(
      endereco
    );

    byte erro =
      Wire.endTransmission();


    if (erro == 0) {

      Serial.print(
        "Dispositivo encontrado: 0x"
      );

      if (endereco < 16)
        Serial.print("0");

      Serial.println(
        endereco,
        HEX
      );

      encontrados++;

    }

  }

  Serial.print(
    "Total encontrado: "
  );

  Serial.println(
    encontrados
  );

  // ===================================================
  // OLED 1
  // ===================================================
  if (
    !display1.begin(
      SSD1306_SWITCHCAPVCC,
      OLED1_ADDRESS
    )
  ) {
    Serial.println(
      "ERRO OLED 1 - 0x3D"
    );

  }

  else {
    Serial.println(
      "OLED 1 OK - 0x3D"
    );

  }


  // ===================================================
  // OLED 2
  // ===================================================
  if (
    !display2.begin(
      SSD1306_SWITCHCAPVCC,
      OLED2_ADDRESS
    )
  ) {
    Serial.println(
      "ERRO OLED 2 - 0x3C"
    );

  }

  else {
    Serial.println(
      "OLED 2 OK - 0x3C"
    );

  }

  // ===================================================
  // ENCODER
  // ===================================================
  pinMode(
    ENC_S1,
    INPUT_PULLUP
  );

  pinMode(
    ENC_S2,
    INPUT_PULLUP
  );

  pinMode(
    ENC_KEY,
    INPUT_PULLUP
  );


  ultimoS1 =
    digitalRead(
      ENC_S1
    );

  // ===================================================
  // PRIMEIRA LEITURA
  // ===================================================
  temperatura =
    termopar.readCelsius();

  // ===================================================
  // TELAS INICIAIS
  // ===================================================
  atualizarDisplay1();
  atualizarDisplay2Status();

  Serial.println();
  Serial.println(
    "Sistema iniciado."
  );

  Serial.println(
    "Gire o encoder para navegar."
  );

  Serial.println(
    "Pressione para entrar."
  );

}

// =====================================================
// LOOP
// =====================================================
void loop() {

  verificarEncoder();

  verificarBotao();

  lerTemperatura();

  delay(2);

}