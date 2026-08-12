#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"
#include "estado.h"
#include "wifi.h"
#include "mqtt.h"

static WiFiClient clienteWiFi;
static PubSubClient mqtt(clienteWiFi);

static unsigned long ultimaPublicacao = 0;
static const unsigned long INTERVALO_PUBLICACAO = 5000;

// =====================================================
// AUXILIAR
// =====================================================

static bool textoLigado(const String &texto) {

  String valor = texto;
  valor.toUpperCase();

  return valor == "ON" ||
         valor == "1" ||
         valor == "LIGADO" ||
         valor == "TRUE";
}

// =====================================================
// RECEBIMENTO DE COMANDOS MQTT
// =====================================================

static void receberMQTT(char* topic, byte* payload, unsigned int length) {

  String mensagem;

  for (unsigned int i = 0; i < length; i++)
    mensagem += (char)payload[i];

  Serial.print("MQTT recebido [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem);

  bool ligado = textoLigado(mensagem);

  if (String(topic) == MQTT_TOPIC_CMD_LUZ) {

    luz = ligado;
  }

  else if (String(topic) == MQTT_TOPIC_CMD_EXAUSTOR) {

    exaustor = ligado;
  }

  else if (String(topic) == MQTT_TOPIC_CMD_SOPRADOR) {

    soprador = ligado;
  }

  // MQTT_TOPIC_CMD_LED é tratado futuramente pelo módulo
  // da luminária. Ele não altera o estado da churrasqueira.
}

// =====================================================
// CONEXÃO MQTT
// =====================================================

static void conectarMQTT() {

  if (!wifiConectado())
    return;

  while (!mqtt.connected()) {

    Serial.print("Conectando ao MQTT...");

    String clientId = "Churrasqueira-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    bool conectado;

    if (String(MQTT_USER).length() > 0) {

      conectado = mqtt.connect(
        clientId.c_str(),
        MQTT_USER,
        MQTT_PASSWORD
      );

    } else {

      conectado = mqtt.connect(clientId.c_str());
    }

    if (conectado) {

      Serial.println(" conectado!");

      mqtt.subscribe(MQTT_TOPIC_CMD_LUZ);
      mqtt.subscribe(MQTT_TOPIC_CMD_EXAUSTOR);
      mqtt.subscribe(MQTT_TOPIC_CMD_SOPRADOR);

      Serial.println("Assinaturas MQTT realizadas.");

      publicarMQTT();

    } else {

      Serial.print(" falhou, estado=");
      Serial.println(mqtt.state());

      // Não trava o programa esperando MQTT.
      // Tenta novamente no próximo ciclo.
      return;
    }
  }
}

// =====================================================
// INICIALIZAÇÃO
// =====================================================

void iniciarMQTT() {

  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(receberMQTT);

  Serial.print("Broker MQTT: ");
  Serial.print(MQTT_BROKER);
  Serial.print(":");
  Serial.println(MQTT_PORT);
}

// =====================================================
// LOOP MQTT
// =====================================================

void verificarMQTT() {

  if (!wifiConectado())
    return;

  if (!mqtt.connected()) {

    conectarMQTT();
    return;
  }

  mqtt.loop();

  if (millis() - ultimaPublicacao >= INTERVALO_PUBLICACAO) {

    ultimaPublicacao = millis();

    publicarMQTT();
  }
}

// =====================================================
// PUBLICAÇÃO DOS ESTADOS
// =====================================================

void publicarMQTT() {

  if (!mqtt.connected())
    return;

  char valor[16];

  dtostrf(temperatura, 1, 1, valor);
  mqtt.publish(MQTT_TOPIC_TEMP, valor, true);

  mqtt.publish(
    MQTT_TOPIC_LUZ,
    luz ? "ON" : "OFF",
    true
  );

  mqtt.publish(
    MQTT_TOPIC_EXAUSTOR,
    exaustor ? "ON" : "OFF",
    true
  );

  mqtt.publish(
    MQTT_TOPIC_SOPRADOR,
    soprador ? "ON" : "OFF",
    true
  );
}
