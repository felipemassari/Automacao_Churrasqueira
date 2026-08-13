#include "Connectivity.h"

/*
  ============================================================
  WIFI / MQTT - NÃO ATIVADO NESTA VERSÃO
  ============================================================

  Quando for usar Wi-Fi, descomente e configure:

  #include <WiFi.h>

  const char* WIFI_SSID = "SEU_WIFI";
  const char* WIFI_SENHA = "SUA_SENHA";

  Para MQTT, posteriormente:

  #include <PubSubClient.h>

  WiFiClient espClient;
  PubSubClient mqttClient(espClient);

  Aqui serão configurados:
    - broker MQTT
    - porta
    - usuário/senha, se houver
    - tópicos
    - publicação da temperatura BBQ
    - temperatura/umidade DHT
    - estados dos equipamentos
    - comandos recebidos do Home Assistant

  O código está propositalmente desativado para não interferir
  nos testes atuais.
*/

void iniciarConectividade() {
  // FUTURO: WiFi + MQTT
}

void processarConectividade() {
  // FUTURO: manutenção da conexão e MQTT
}
