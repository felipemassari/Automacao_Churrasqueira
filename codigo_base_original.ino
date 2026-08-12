#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

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
// MENU
// =====================================================

const char* menu[] = {
  "Luz",
  "Exaustor",
  "Soprador",
  "LED",
  "Config"
};

const int MENU_TOTAL = 5;

int menuAtual = 0;

// =====================================================
// ESTADOS SIMULADOS
// Por enquanto não acionam relés.
// =====================================================

bool luz = false;
bool exaustor = false;
bool soprador = false;
bool led = false;

// =====================================================
// ENCODER
// =====================================================

int ultimoS1 = HIGH;

unsigned long ultimoClique = 0;

#define DEBOUNCE 250

// =====================================================
// TEMPERATURA
// =====================================================

float temperatura = 0;

unsigned long ultimaLeituraTemp = 0;

// =====================================================
// DESENHA ÍCONE DA LUZ
// =====================================================

void iconeLuz(int x, int y, bool ligado) {

  if (!ligado) {
    // vazio quando desligado
    return;
  }

  display1.drawCircle(x, y, 5, SSD1306_WHITE);

  display1.drawLine(x - 3, y + 5, x + 3, y + 5, SSD1306_WHITE);
  display1.drawLine(x - 2, y + 7, x + 2, y + 7, SSD1306_WHITE);

  // raios
  display1.drawLine(x, y - 8, x, y - 11, SSD1306_WHITE);
  display1.drawLine(x - 7, y - 6, x - 9, y - 8, SSD1306_WHITE);
  display1.drawLine(x + 7, y - 6, x + 9, y - 8, SSD1306_WHITE);
  display1.drawLine(x - 8, y, x - 11, y, SSD1306_WHITE);
  display1.drawLine(x + 8, y, x + 11, y, SSD1306_WHITE);
}

// =====================================================
// DESENHA ÍCONE DO EXAUSTOR
// =====================================================

void iconeExaustor(int x, int y, bool ligado) {

  if (!ligado) {
    return;
  }

  display1.drawCircle(x, y, 2, SSD1306_WHITE);

  display1.drawLine(x, y - 2, x, y - 9, SSD1306_WHITE);
  display1.drawLine(x + 2, y, x + 9, y - 3, SSD1306_WHITE);
  display1.drawLine(x, y + 2, x + 4, y + 8, SSD1306_WHITE);
  display1.drawLine(x - 2, y, x - 9, y + 3, SSD1306_WHITE);
}

// =====================================================
// DESENHA ÍCONE DO SOPRADOR
// =====================================================

void iconeSoprador(int x, int y, bool ligado) {

  if (!ligado) {
    return;
  }

  // corpo
  display1.drawRect(
    x - 8,
    y - 5,
    8,
    10,
    SSD1306_WHITE
  );

  // saída
  display1.drawLine(
    x,
    y - 3,
    x + 7,
    y - 3,
    SSD1306_WHITE
  );

  display1.drawLine(
    x,
    y + 3,
    x + 7,
    y + 3,
    SSD1306_WHITE
  );

  // vento
  display1.drawLine(
    x + 9,
    y - 5,
    x + 13,
    y - 5,
    SSD1306_WHITE
  );

  display1.drawLine(
    x + 9,
    y,
    x + 14,
    y,
    SSD1306_WHITE
  );

  display1.drawLine(
    x + 9,
    y + 5,
    x + 13,
    y + 5,
    SSD1306_WHITE
  );
}

// =====================================================
// ÍCONE DO LED
// =====================================================

void iconeLED(int x, int y, bool ligado) {

  if (!ligado) {
    return;
  }

  display1.drawCircle(x, y, 5, SSD1306_WHITE);

  display1.drawLine(x, y - 9, x, y - 6, SSD1306_WHITE);
  display1.drawLine(x, y + 6, x, y + 9, SSD1306_WHITE);
  display1.drawLine(x - 9, y, x - 6, y, SSD1306_WHITE);
  display1.drawLine(x + 6, y, x + 9, y, SSD1306_WHITE);
}

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

  if (temperatura >= 0 && temperatura < 1000) {

    display1.print((int)temperatura);

  } else {

    display1.print("---");
  }

  display1.setTextSize(1);

  display1.setCursor(91, 28);
  display1.print("C");

  // -------------------------
  // Linha separadora
  // -------------------------

  display1.drawLine(
    0,
    47,
    127,
    47,
    SSD1306_WHITE
  );

  // -------------------------
  // Ícones
  // -------------------------

  iconeLuz(18, 56, luz);

  iconeExaustor(55, 56, exaustor);

  iconeSoprador(91, 56, soprador);

  iconeLED(119, 56, led);

  display1.display();
}

// =====================================================
// DISPLAY 2 - ITEM DO MENU
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

    bool estado = false;

    if (menuAtual == 0)
      estado = luz;

    if (menuAtual == 1)
      estado = exaustor;

    if (menuAtual == 2)
      estado = soprador;

    if (menuAtual == 3)
      estado = led;

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
  // Indicador de navegação
  // -------------------------

  display2.setTextSize(1);

  display2.setCursor(0, 56);
  display2.print("<");

  display2.setCursor(120, 56);
  display2.print(">");

  display2.display();
}

// =====================================================
// CLIQUE DO ENCODER
// =====================================================

void verificarBotao() {

  int estado = digitalRead(ENC_KEY);

  if (estado == LOW) {

    if (millis() - ultimoClique > DEBOUNCE) {

      ultimoClique = millis();

      // -------------------------
      // Luz
      // -------------------------

      if (menuAtual == 0) {

        luz = !luz;

        Serial.print("Luz: ");
        Serial.println(luz ? "ON" : "OFF");
      }

      // -------------------------
      // Exaustor
      // -------------------------

      else if (menuAtual == 1) {

        exaustor = !exaustor;

        Serial.print("Exaustor: ");
        Serial.println(exaustor ? "ON" : "OFF");
      }

      // -------------------------
      // Soprador
      // -------------------------

      else if (menuAtual == 2) {

        soprador = !soprador;

        Serial.print("Soprador: ");
        Serial.println(soprador ? "ON" : "OFF");
      }

      // -------------------------
      // LED
      // -------------------------

      else if (menuAtual == 3) {

        led = !led;

        Serial.print("LED: ");
        Serial.println(led ? "ON" : "OFF");
      }

      // -------------------------
      // CONFIG
      // -------------------------

      else if (menuAtual == 4) {

        Serial.println("CONFIG selecionado");

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

        delay(1000);
      }

      atualizarDisplay1();
      atualizarDisplay2();
    }
  }
}

// =====================================================
// ROTAÇÃO DO ENCODER
// =====================================================

void verificarEncoder() {

  int s1 = digitalRead(ENC_S1);

  if (s1 != ultimoS1) {

    // Só processa na descida
    if (s1 == LOW) {

      int s2 = digitalRead(ENC_S2);

      if (s2 != s1) {

        // horário
        menuAtual++;

        if (menuAtual >= MENU_TOTAL)
          menuAtual = 0;

      } else {

        // anti-horário
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

// =====================================================
// LEITURA DA TEMPERATURA
// =====================================================

void lerTemperatura() {

  if (millis() - ultimaLeituraTemp < 1000)
    return;

  ultimaLeituraTemp = millis();

  temperatura = termopar.readCelsius();

  Serial.print("Temperatura churrasqueira: ");
  Serial.print(temperatura);
  Serial.println(" C");

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
  Serial.println(" TESTE CHURRASQUEIRA");
  Serial.println("==============================");

  // ===================================================
  // I2C
  // ===================================================

  Wire.begin(SDA_PIN, SCL_PIN);

  delay(100);

  // ===================================================
  // Scanner I2C
  // ===================================================

  byte encontrados = 0;

  Serial.println("Procurando displays I2C...");

  for (byte endereco = 1; endereco < 127; endereco++) {

    Wire.beginTransmission(endereco);

    byte erro = Wire.endTransmission();

    if (erro == 0) {

      Serial.print("Encontrado: 0x");

      if (endereco < 16)
        Serial.print("0");

      Serial.println(endereco, HEX);

      encontrados++;
    }
  }

  Serial.print("Total: ");
  Serial.println(encontrados);

  // ===================================================
  // OLED 1 - 0x3D
  // ===================================================

  if (!display1.begin(
        SSD1306_SWITCHCAPVCC,
        OLED1_ADDRESS
      )) {

    Serial.println("ERRO OLED 1 - 0x3D");

  } else {

    Serial.println("OLED 1 OK - 0x3D");
  }

  // ===================================================
  // OLED 2 - 0x3C
  // ===================================================

  if (!display2.begin(
        SSD1306_SWITCHCAPVCC,
        OLED2_ADDRESS
      )) {

    Serial.println("ERRO OLED 2 - 0x3C");

  } else {

    Serial.println("OLED 2 OK - 0x3C");
  }

  // ===================================================
  // ENCODER
  // ===================================================

  pinMode(ENC_S1, INPUT_PULLUP);
  pinMode(ENC_S2, INPUT_PULLUP);
  pinMode(ENC_KEY, INPUT_PULLUP);

  ultimoS1 = digitalRead(ENC_S1);

  // ===================================================
  // PRIMEIRA LEITURA
  // ===================================================

  temperatura = termopar.readCelsius();

  atualizarDisplay1();
  atualizarDisplay2();

  Serial.println();
  Serial.println("Sistema iniciado.");
  Serial.println("Gire o encoder para navegar.");
  Serial.println("Pressione para alterar o estado.");
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
