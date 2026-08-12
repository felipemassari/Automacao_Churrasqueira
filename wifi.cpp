#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "wifi.h"

void iniciarWiFi() {

  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long inicio = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - inicio < 15000) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("Wi-Fi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

  } else {

    Serial.println("Falha ao conectar no Wi-Fi.");
    Serial.println("O programa continua funcionando localmente.");
  }
}

bool wifiConectado() {
  return WiFi.status() == WL_CONNECTED;
}
