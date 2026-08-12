#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "i2c.h"

void iniciarI2C() {

  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);
}

void scannerI2C() {

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

  Serial.print("Total: ");
  Serial.println(encontrados);
}
