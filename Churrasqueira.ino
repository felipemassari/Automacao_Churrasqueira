#include <Arduino.h>

#include "config.h"
#include "estado.h"
#include "i2c.h"
#include "displays.h"
#include "encoder.h"
#include "temperatura.h"
#include "wifi.h"
#include "mqtt.h"

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" TESTE CHURRASQUEIRA");
  Serial.println("==============================");

  // I2C
  iniciarI2C();

  // Scanner mantido no projeto para facilitar testes
  // dos dois OLEDs e demais dispositivos I2C.
  scannerI2C();

  // Wi-Fi e MQTT
  iniciarWiFi();
  iniciarMQTT();

  // Displays
  iniciarDisplays();

  // Encoder
  iniciarEncoder();

  // Primeira leitura do MAX6675
  iniciarTemperatura();

  // Telas iniciais
  atualizarDisplay1();
  atualizarDisplay2();

  Serial.println();
  Serial.println("Sistema iniciado.");
  Serial.println("Gire o encoder para navegar.");
  Serial.println("Pressione para alterar o estado.");
}

void loop() {

  verificarEncoder();

  verificarBotao();

  lerTemperatura();

  verificarMQTT();

  delay(2);
}
