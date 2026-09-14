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

const char* MQTT_USER  = "esp32Churras";
const char* MQTT_SENHA = "brasaqueima128";

WiFiClient espClient;
PubSubClient mqttClient(espClient);


// ============================================================
// TEMPORIZAÇÃO
// ============================================================
unsigned long ultimoMQTT = 0;
const unsigned long INTERVALO_MQTT = 180000UL;


// ============================================================
// LED DE CONEXÃO
// ============================================================
int falhasMQTT = 0;
bool alertaMQTT = false;
unsigned long ultimoPiscaLED = 0;
bool estadoLED = false;


// ============================================================
// CONTROLE DO LED
// ============================================================
void ledApagar()
{
  digitalWrite(LED_CONNECT, LOW);
  estadoLED = false;
}


void ledAcender()
{
  digitalWrite(LED_CONNECT, HIGH);
  estadoLED = true;
}


// ============================================================
// LED PISCANDO DURANTE CONEXÃO WIFI
// ============================================================
void ledPiscandoConexao()
{
  unsigned long agora = millis();

  if (agora - ultimoPiscaLED >= 500)
  {
    ultimoPiscaLED = agora;

    estadoLED = !estadoLED;
    digitalWrite(LED_CONNECT, estadoLED ? HIGH : LOW);
  }
}


// ============================================================
// LED PISCANDO EM ALERTA MQTT
// ============================================================
void ledPiscandoAlerta()
{
  unsigned long agora = millis();

  if (agora - ultimoPiscaLED >= 350)
  {
    ultimoPiscaLED = agora;

    estadoLED = !estadoLED;
    digitalWrite(LED_CONNECT, estadoLED ? HIGH : LOW);
  }
}


// ============================================================
// PISCAR 3X QUANDO MQTT PUBLICAR COM SUCESSO
// ============================================================
void ledPublicacaoOK()
{
  for (int i = 0; i < 3; i++)
  {
    ledApagar();
    delay(500);

    ledAcender();
    delay(500);
  }

  // Depois das 3 piscadas, fica aceso
  ledAcender();
}


// ============================================================
// INICIAR CONECTIVIDADE
// ============================================================
void iniciarConectividade()
{
  Serial.println();
  Serial.println("===== CONECTIVIDADE =====");

  // LED inicialmente apagado
  pinMode(LED_CONNECT, OUTPUT);
  ledApagar();

  // WIFI
  Serial.print("Conectando WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false);
  delay(500);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_SENHA);

  unsigned long tempoInicio = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    // Pisca junto com os pontos da Serial
    estadoLED = !estadoLED;
    digitalWrite(LED_CONNECT, estadoLED ? HIGH : LOW);

    if (millis() - tempoInicio >= 30000UL)
    {
      Serial.println();
      Serial.println("Falha ao conectar no WiFi.");

      wifiConnected = false;

      // WiFi não conectou -> LED apagado
      ledApagar();

      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.println("WiFi conectado");

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    wifiConnected = true;

    // WiFi conectado -> LED aceso
    ledAcender();
  }

  // MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(callbackMQTT);

  // AUMENTA O BUFFER DO PUBSUBCLIENT
  mqttClient.setBufferSize(2048);

  Serial.println("MQTT configurado");

  Serial.print("Buffer MQTT: ");
  Serial.println(mqttClient.getBufferSize());
}


// ============================================================
// CONECTAR MQTT
// ============================================================
void conectarMQTT()
{
  if (mqttClient.connected())
    return;

  Serial.print("Conectando MQTT... ");

  if (mqttClient.connect(
        "ESP32ChurrasqueiraClient",
        MQTT_USER,
        MQTT_SENHA))
  {
    Serial.println("OK");

    // MQTT DISCOVERY
    publicarDiscovery();

    // ASSINAR COMANDOS DO HOME ASSISTANT
    mqttClient.subscribe("churrasqueira/luz/set");
    mqttClient.subscribe("churrasqueira/exaustor/set");
    mqttClient.subscribe("churrasqueira/soprador/set");
    mqttClient.subscribe("churrasqueira/led_mureta/set");

    Serial.println("Topicos MQTT de comando assinados");

    // MQTT conectou -> LED aceso
    alertaMQTT = false;
    falhasMQTT = 0;
    ledAcender();
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
void publicarDiscovery()
{
  if (!mqttClient.connected())
  {
    Serial.println("Discovery: MQTT desconectado");
    return;
  }

  Serial.println();
  Serial.println("===== MQTT DISCOVERY =====");

  Serial.print("Buffer MQTT atual: ");
  Serial.println(mqttClient.getBufferSize());


  // ==========================================================
  // DEVICE
  // ==========================================================
  String devInfo =
    ",\"device\":{"
      "\"identifiers\":[\"esp32_churrasqueira_v2\"],"
      "\"name\":\"Churrasqueira ESP32\","
      "\"model\":\"ESP32 Custom\","
      "\"manufacturer\":\"DIY\""
    "}";


  // ==========================================================
  // LUZ
  // ==========================================================
  String payloadLuz =
    "{"
      "\"name\":\"Luz Churrasqueira\","
      "\"unique_id\":\"bbq_luz_01\","
      "\"command_topic\":\"churrasqueira/luz/set\","
      "\"state_topic\":\"churrasqueira/luz\","
      "\"payload_on\":\"ON\","
      "\"payload_off\":\"OFF\""
      + devInfo +
    "}";

  Serial.print("Tamanho payload LUZ: ");
  Serial.println(payloadLuz.length());

  bool okLuz = mqttClient.publish(
    "homeassistant/switch/churrasqueira/luz/config",
    payloadLuz.c_str(),
    true
  );

  Serial.print("Discovery LUZ: ");
  Serial.println(okLuz ? "OK" : "FALHOU");


  // ==========================================================
  // EXAUSTOR
  // ==========================================================
  String payloadExaustor =
    "{"
      "\"name\":\"Exaustor Churrasqueira\","
      "\"unique_id\":\"bbq_exaustor_01\","
      "\"command_topic\":\"churrasqueira/exaustor/set\","
      "\"state_topic\":\"churrasqueira/exaustor\","
      "\"payload_on\":\"ON\","
      "\"payload_off\":\"OFF\""
      + devInfo +
    "}";

  Serial.print("Tamanho payload EXAUSTOR: ");
  Serial.println(payloadExaustor.length());

  bool okExaustor = mqttClient.publish(
    "homeassistant/switch/churrasqueira/exaustor/config",
    payloadExaustor.c_str(),
    true
  );

  Serial.print("Discovery EXAUSTOR: ");
  Serial.println(okExaustor ? "OK" : "FALHOU");


  // ==========================================================
  // SOPRADOR
  // ==========================================================
  String payloadSoprador =
    "{"
      "\"name\":\"Soprador Churrasqueira\","
      "\"unique_id\":\"bbq_soprador_01\","
      "\"command_topic\":\"churrasqueira/soprador/set\","
      "\"state_topic\":\"churrasqueira/soprador\","
      "\"payload_on\":\"ON\","
      "\"payload_off\":\"OFF\""
      + devInfo +
    "}";

  Serial.print("Tamanho payload SOPRADOR: ");
  Serial.println(payloadSoprador.length());

  bool okSoprador = mqttClient.publish(
    "homeassistant/switch/churrasqueira/soprador/config",
    payloadSoprador.c_str(),
    true
  );

  Serial.print("Discovery SOPRADOR: ");
  Serial.println(okSoprador ? "OK" : "FALHOU");


  // ==========================================================
  // LED MURETA
  // ==========================================================
  String payloadLed =
    "{"
      "\"name\":\"LED Mureta\","
      "\"unique_id\":\"bbq_led_01\","
      "\"command_topic\":\"churrasqueira/led_mureta/set\","
      "\"state_topic\":\"churrasqueira/led_mureta\","
      "\"payload_on\":\"ON\","
      "\"payload_off\":\"OFF\""
      + devInfo +
    "}";

  Serial.print("Tamanho payload LED: ");
  Serial.println(payloadLed.length());

  bool okLed = mqttClient.publish(
    "homeassistant/switch/churrasqueira/led/config",
    payloadLed.c_str(),
    true
  );

  Serial.print("Discovery LED: ");
  Serial.println(okLed ? "OK" : "FALHOU");


  // ==========================================================
  // TEMPERATURA DA CHURRASQUEIRA
  // ==========================================================
  String payloadTemperatura =
    "{"
      "\"name\":\"Temperatura Churrasqueira\","
      "\"unique_id\":\"bbq_temperatura_01\","
      "\"state_topic\":\"churrasqueira/temperatura\","
      "\"unit_of_measurement\":\"°C\","
      "\"device_class\":\"temperature\","
      "\"state_class\":\"measurement\""
      + devInfo +
    "}";

  Serial.print("Tamanho payload TEMPERATURA: ");
  Serial.println(payloadTemperatura.length());

  bool okTemperatura = mqttClient.publish(
    "homeassistant/sensor/churrasqueira/temperatura/config",
    payloadTemperatura.c_str(),
    true
  );

  Serial.print("Discovery TEMPERATURA: ");
  Serial.println(okTemperatura ? "OK" : "FALHOU");

  Serial.println("==========================");
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

  if (strcmp(topic, "churrasqueira/luz/set") == 0)
  {
    luz = (mensagem == "ON");
    digitalWrite(RELE_LUZ, luz ? LOW : HIGH);
    atualizarDisplay1();
  }
  else if (strcmp(topic, "churrasqueira/exaustor/set") == 0)
  {
    exaustor = (mensagem == "ON");
    digitalWrite(RELE_EXAUSTOR, exaustor ? LOW : HIGH);
    atualizarDisplay1();
  }
  else if (strcmp(topic, "churrasqueira/soprador/set") == 0)
  {
    soprador = (mensagem == "ON");
    digitalWrite(RELE_SOPRADOR, soprador ? LOW : HIGH);
    atualizarDisplay1();
  }
  else if (strcmp(topic, "churrasqueira/led_mureta/set") == 0)
  {
    ledMureta = (mensagem == "ON");
    digitalWrite(RELE_LED, ledMureta ? LOW : HIGH);
  }

  publicarMQTT();
}


// ============================================================
// SINCRONIZAR ESTADOS MQTT
// ============================================================
bool sincronizarMQTT()
{
  if (!mqttClient.connected())
    return false;

  Serial.println("Sincronizando estados MQTT...");

  bool sucesso = true;

  if (!mqttClient.publish(
        "churrasqueira/luz",
        luz ? "ON" : "OFF",
        true))
  {
    sucesso = false;
  }

  if (!mqttClient.publish(
        "churrasqueira/exaustor",
        exaustor ? "ON" : "OFF",
        true))
  {
    sucesso = false;
  }

  if (!mqttClient.publish(
        "churrasqueira/soprador",
        soprador ? "ON" : "OFF",
        true))
  {
    sucesso = false;
  }

  if (!mqttClient.publish(
        "churrasqueira/led_mureta",
        ledMureta ? "ON" : "OFF",
        true))
  {
    sucesso = false;
  }

  return sucesso;
}


// ============================================================
// PUBLICAR DADOS
// ============================================================
void publicarMQTT()
{
  if (!mqttClient.connected())
    return;

  bool sucesso = sincronizarMQTT();

  // TEMPERATURA DA CHURRASQUEIRA
  String temp = String(temperatura, 1);

  if (!mqttClient.publish(
        "churrasqueira/temperatura",
        temp.c_str(),
        true))
  {
    sucesso = false;
  }

  // ==========================================================
  // CONTROLE DE FALHAS MQTT
  // ==========================================================
  if (sucesso)
  {
    falhasMQTT = 0;

    if (alertaMQTT)
    {
      alertaMQTT = false;
    }

    ledPublicacaoOK();
  }
  else
  {
    falhasMQTT++;

    Serial.print("Falha MQTT: ");
    Serial.print(falhasMQTT);
    Serial.println("/5");

    if (falhasMQTT >= 5)
    {
      alertaMQTT = true;

      Serial.println("ALERTA: 5 falhas MQTT consecutivas");
    }
  }
}

// ============================================================
// PROCESSAR CONECTIVIDADE
// ============================================================
void processarConectividade()
{
  // ==========================================================
  // WIFI DESCONECTADO
  // ==========================================================
  if (WiFi.status() != WL_CONNECTED)
  {
    wifiConnected = false;

    // LED apagado
    ledApagar();

    Serial.println("WiFi desconectado");
    return;
  }


  // ==========================================================
  // WIFI CONECTADO
  // ==========================================================
  wifiConnected = true;

  // ==========================================================
  // MQTT DESCONECTADO
  // ==========================================================
  if (!mqttClient.connected())
  {
    conectarMQTT();

    if (mqttClient.connected())
    {
      sincronizarMQTT();

      ultimoMQTT = millis();

      falhasMQTT = 0;
      alertaMQTT = false;

      ledAcender();
    }
  }

  // ==========================================================
  // MQTT CONECTADO
  // ==========================================================
  mqttClient.loop();


  // ==========================================================
  // LED DE ALERTA MQTT
  // ==========================================================
  if (alertaMQTT)
  {
    ledPiscandoAlerta();
  }
  else
  {
    ledAcender();
  }


  // ==========================================================
  // PUBLICAÇÃO PERIÓDICA
  // ==========================================================
  if (millis() - ultimoMQTT >= INTERVALO_MQTT)
  {
    ultimoMQTT = millis();

    publicarMQTT();
  }
}