#include <Arduino.h>
#include <max6675.h>

#include "config.h"
#include "estado.h"
#include "displays.h"

static MAX6675 termopar(MAX_SCK, MAX_CS, MAX_SO);
static unsigned long ultimaLeituraTemp = 0;

void iniciarTemperatura() {
  temperatura = termopar.readCelsius();
}

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
