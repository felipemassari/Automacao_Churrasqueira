#ifndef CONFIG_H
#define CONFIG_H

// =====================================================
// CONFIGURAÇÃO DE HARDWARE
// =====================================================

#define SDA_PIN 3
#define SCL_PIN 1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// OLED 1 = Display principal
#define OLED1_ADDRESS 0x3D

// OLED 2 = Display/menu
#define OLED2_ADDRESS 0x3C

// =====================================================
// MAX6675
// =====================================================

#define MAX_SCK 0
#define MAX_CS  2
#define MAX_SO  21

// =====================================================
// ENCODER
// S1 = CLK
// S2 = DT
// KEY = botão
// =====================================================

#define ENC_S1  10
#define ENC_S2  8
#define ENC_KEY 6

#define DEBOUNCE 250

// =====================================================
// WI-FI
// =====================================================

#define WIFI_SSID "SEU_WIFI"
#define WIFI_PASSWORD "SUA_SENHA"

// =====================================================
// MQTT
// =====================================================

#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883

#define MQTT_USER ""
#define MQTT_PASSWORD ""

// Tópicos da churrasqueira
#define MQTT_TOPIC_TEMP       "churrasqueira/temperatura"
#define MQTT_TOPIC_LUZ        "churrasqueira/luz"
#define MQTT_TOPIC_EXAUSTOR   "churrasqueira/exaustor"
#define MQTT_TOPIC_SOPRADOR   "churrasqueira/soprador"

// Comandos recebidos
#define MQTT_TOPIC_CMD_LUZ       "churrasqueira/cmd/luz"
#define MQTT_TOPIC_CMD_EXAUSTOR  "churrasqueira/cmd/exaustor"
#define MQTT_TOPIC_CMD_SOPRADOR  "churrasqueira/cmd/soprador"

// Luminária LED é independente da automação da churrasqueira.
// O tópico fica separado e não entra no menu/estado da churrasqueira.
#define MQTT_TOPIC_CMD_LED       "luminaria/led"

#endif
