#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "State.h"
#include "Config.h"
#include "Connectivity.h"

// ============================================================
// WIFI
// ============================================================
const char* WIFI_SSID  = "Skynet";
const char* WIFI_SENHA = "F&p@0128";
bool wifiConnected = false;

// ============================================================
// MQTT
// ============================================================
const char* MQTT_BROKER = "192.168.68.136";
const int   MQTT_PORT   = 1883;

const char* MQTT_USER = "esp32Churras";
const char* MQTT_SENHA = "brasaqueima128";

#define MQTT_MAX_PACKET_SIZE 512
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ============================================================
// TEMPORIZAÇÃO
// ============================================================
unsigned long ultimoMQTT = 0;
const unsigned long INTERVALO_MQTT = 180000UL;


// ============================================================
// INICIAR CONECTIVIDADE
// ============================================================
void iniciarConectividade()
{
  Serial.println();
  Serial.println("===== CONECTIVIDADE =====");

  // ----------------------------
  // WIFI
  // ----------------------------
  Serial.print("Conectando WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.persistent(false);
  WiFi.disconnect(true);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  unsigned long tempoInicio = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    if (millis() - tempoInicio >= 30000UL)
    {
      Serial.println();
      Serial.println("Falha ao conectar no WiFi.");
      Serial.println("Continuando sem WiFi...");
      break;
    }
  }

  // ----------------------------
  // RESULTADO WIFI
  // ----------------------------
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.println("WiFi conectado");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    wifiConnected = true;
  }

  // ----------------------------
  // MQTT
  // ----------------------------
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(callbackMQTT);

  Serial.println("MQTT configurado");
}
// ============================================================
// CONECTAR MQTT
// ============================================================
void conectarMQTT()
{
  if (mqttClient.connected())
    return;

  Serial.print("Conectando MQTT... ");

  if (mqttClient.connect("ESP32ChurrasqueiraClient", MQTT_USER, MQTT_SENHA))
  {
    Serial.println("OK");

    // ----------------------------------------
    // MQTT DISCOVERY
    // ----------------------------------------
    publicarDiscovery();

    // ----------------------------------------
    // ASSINAR COMANDOS DO HOME ASSISTANT
    // ----------------------------------------
    mqttClient.subscribe("churrasqueira/luz/set");
    mqttClient.subscribe("churrasqueira/exaustor/set");
    mqttClient.subscribe("churrasqueira/soprador/set");
    mqttClient.subscribe("churrasqueira/led_mureta/set");

    Serial.println("Topicos MQTT de comando assinados");
  }
  else
  {
    Serial.print("ERRO: ");
    Serial.println(mqttClient.state());
  }
}

// ============================================================
// MQTT DISCOVERY
// ============================================================
void publicarDiscovery(){
  if (!mqttClient.connected())
    return;

  Serial.println("Enviando MQTT Discovery...");

  // ----------------------------
  // LUZ
  // ----------------------------
  mqttClient.publish(
    "homeassistant/switch/churrasqueira_luz/config",
    "{\"name\":\"Luz Churrasqueira\",\"unique_id\":\"churrasqueira_luz\",\"command_topic\":\"churrasqueira/luz/set\",\"state_topic\":\"churrasqueira/luz\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"state_on\":\"ON\",\"state_off\":\"OFF\"}",
    true
  );

  // ----------------------------
  // EXAUSTOR
  // ----------------------------
  mqttClient.publish(
    "homeassistant/switch/churrasqueira_exaustor/config",
    "{\"name\":\"Exaustor Churrasqueira\",\"unique_id\":\"churrasqueira_exaustor\",\"command_topic\":\"churrasqueira/exaustor/set\",\"state_topic\":\"churrasqueira/exaustor\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"state_on\":\"ON\",\"state_off\":\"OFF\"}",
    true
  );

  // ----------------------------
  // SOPRADOR
  // ----------------------------
  mqttClient.publish(
    "homeassistant/switch/churrasqueira_soprador/config",
    "{\"name\":\"Soprador Churrasqueira\",\"unique_id\":\"churrasqueira_soprador\",\"command_topic\":\"churrasqueira/soprador/set\",\"state_topic\":\"churrasqueira/soprador\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"state_on\":\"ON\",\"state_off\":\"OFF\"}",
    true
  );

  // ----------------------------
  // LED MURETA
  // ----------------------------
  mqttClient.publish(
    "homeassistant/switch/churrasqueira_led_mureta/config",
    "{\"name\":\"LED Mureta\",\"unique_id\":\"churrasqueira_led_mureta\",\"command_topic\":\"churrasqueira/led_mureta/set\",\"state_topic\":\"churrasqueira/led_mureta\",\"payload_on\":\"ON\",\"payload_off\":\"OFF\",\"state_on\":\"ON\",\"state_off\":\"OFF\"}",
    true
  );

  Serial.println("MQTT Discovery enviado");
}

// ============================================================
// CALLBACK MQTT
// ============================================================
void callbackMQTT(char* topic, byte* payload, unsigned int length)
{
  String mensagem;

  for (unsigned int i = 0; i < length; i++)
  {
    mensagem += (char)payload[i];
  }

  Serial.print("MQTT comando: ");
  Serial.print(topic);
  Serial.print(" -> ");
  Serial.println(mensagem);

  // ==========================================================
  // LUZ
  // ==========================================================
  if (strcmp(topic, "churrasqueira/luz/set") == 0)
  {
    luz = (mensagem == "ON");

    digitalWrite(RELE_LUZ, luz ? LOW : HIGH);
  }

  // ==========================================================
  // EXAUSTOR
  // ==========================================================
  else if (strcmp(topic, "churrasqueira/exaustor/set") == 0)
  {
    exaustor = (mensagem == "ON");

    digitalWrite(RELE_EXAUSTOR, exaustor ? LOW : HIGH);
  }

  // ==========================================================
  // SOPRADOR
  // ==========================================================
  else if (strcmp(topic, "churrasqueira/soprador/set") == 0)
  {
    soprador = (mensagem == "ON");

    digitalWrite(RELE_SOPRADOR, soprador ? LOW : HIGH);
  }

  // ==========================================================
  // LED MURETA
  // ==========================================================
  else if (strcmp(topic, "churrasqueira/led_mureta/set") == 0)
  {
    ledMureta = (mensagem == "ON");

    digitalWrite(RELE_LED, ledMureta ? LOW : HIGH);
  }

  // ==========================================================
  // PUBLICA ESTADO ATUAL IMEDIATAMENTE
  // ==========================================================
  publicarMQTT();
}

// ============================================================
// PUBLICAR DADOS
// ============================================================
void publicarMQTT()
{
  if (!mqttClient.connected())    return;
  sincronizarMQTT();
}

// ============================================================
// SINCRONIZAR ESTADOS MQTT
// ============================================================
void sincronizarMQTT()
{
  if (!mqttClient.connected())
    return;

  Serial.println("Sincronizando estados MQTT...");

  mqttClient.publish(
    "churrasqueira/luz",
    luz ? "ON" : "OFF",
    true
  );

  mqttClient.publish(
    "churrasqueira/exaustor",
    exaustor ? "ON" : "OFF",
    true
  );

  mqttClient.publish(
    "churrasqueira/soprador",
    soprador ? "ON" : "OFF",
    true
  );

  mqttClient.publish(
    "churrasqueira/led_mureta",
    ledMureta ? "ON" : "OFF",
    true
  );
}

// ============================================================
// PROCESSAR CONECTIVIDADE
// ============================================================
void processarConectividade()
{
  // ----------------------------
  // Verifica WiFi
  // ----------------------------
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi desconectado");
    return;
  }

  // ----------------------------
  // Verifica MQTT
  // ----------------------------
  if (!mqttClient.connected())
  {
    conectarMQTT();

    // Se acabou de conectar,
    // envia imediatamente os estados atuais
    if (mqttClient.connected())
    {
      sincronizarMQTT();
      ultimoMQTT = millis();
    }
  }

  mqttClient.loop();

  // ----------------------------
  // Publicação periódica
  // ----------------------------
  if (millis() - ultimoMQTT >= INTERVALO_MQTT)
  {
    ultimoMQTT = millis();

    publicarMQTT();
  }
}