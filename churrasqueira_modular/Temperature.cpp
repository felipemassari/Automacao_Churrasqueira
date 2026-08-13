#include "Temperature.h"

#include <Arduino.h>
#include <max6675.h>

#include "State.h"
#include "Display.h"

extern MAX6675 termopar;

void lerTemperatura() {

  if (millis() - ultimaLeituraTemp < 10000)
    return;

  ultimaLeituraTemp = millis();

  temperatura = termopar.readCelsius();

  Serial.print("Temperatura BBQ: ");
  Serial.print(temperatura);
  Serial.println(" C");

  atualizarDisplay1();
}
