#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

#include "Config.h"
#include "State.h"
#include "Display.h"
#include "Encoder.h"
#include "Temperature.h"

// ============================================================
// OBJETOS DE HARDWARE
// ============================================================

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX6675 termopar(MAX_SCK, MAX_CS, MAX_SO);

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("   CHURRASQUEIRA");
  Serial.println("==============================");

  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);

  // ==========================================================
  // SCANNER I2C
  // ==========================================================

  byte encontrados = 0;

  Serial.println("Procurando dispositivos I2C...");

  for (byte endereco = 1; endereco < 127; endereco++) {

    Wire.beginTransmission(endereco);
    byte erro = Wire.endTransmission();

    if (erro == 0) {

      Serial.print("Dispositivo encontrado: 0x");

      if (endereco < 16)
        Serial.print("0");

      Serial.println(endereco, HEX);
      encontrados++;
    }
  }

  Serial.print("Total encontrado: ");
  Serial.println(encontrados);

  // ==========================================================
  // OLED 1
  // ==========================================================

  if (!display1.begin(SSD1306_SWITCHCAPVCC, OLED1_ADDRESS))
    Serial.println("ERRO OLED 1 - 0x3D");
  else
    Serial.println("OLED 1 OK - 0x3D");

  // ==========================================================
  // OLED 2
  // ==========================================================

  if (!display2.begin(SSD1306_SWITCHCAPVCC, OLED2_ADDRESS))
    Serial.println("ERRO OLED 2 - 0x3C");
  else
    Serial.println("OLED 2 OK - 0x3C");

  // ==========================================================
  // ENCODER
  // ==========================================================

  inicializarEncoder();

  // ==========================================================
  // PRIMEIRA LEITURA
  // ==========================================================

  temperatura = termopar.readCelsius();

  atualizarDisplay1();
  atualizarDisplay2Status();

  Serial.println();
  Serial.println("Sistema iniciado.");
  Serial.println("Gire o encoder para navegar.");
  Serial.println("Pressione para entrar.");

  // ==========================================================
  // WIFI / MQTT - FUTURO
  // ==========================================================
  // Quando for usar, habilitar as funções em Connectivity.cpp
  // e descomentar:
  //
  // iniciarConectividade();
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  verificarEncoder();
  verificarBotao();
  lerTemperatura();

  // WIFI / MQTT - FUTURO:
  // processarConectividade();

  delay(2);
}
