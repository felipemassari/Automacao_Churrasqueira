#include "State.h"

// DHT11 ainda não instalado: valores simulados
int tempDHT = 25;
int umidDHT = 60;

bool luz = false;
bool exaustor = false;
bool soprador = false;
bool ledMureta = false;

int setTempExaustor = 35;
int setTempSoprador = 60;

const char* menu[] = {
  "Luz",
  "Exaustor",
  "Soprador",
  "Config"
};

const char* menuConfig[] = {
  "Modo Auto",
  "Time Tela",
  "Temperat. Exaustor",
  "Temperat. Soprador",
  "Led Muro",
  "Voltar"
};

const int MENU_TOTAL = 4;
const int MENU_CONFIG_TOTAL = 6;
int menuAtual = 0;
int menuConfigAtual = 0;
int valorConfigTemp = 0;

bool editandoOpcao = false;
bool dentroConfig = false;
bool estadoTemporario = false;
bool modoAutomatico = false;
bool estadoConfigTemp = false;
bool aguardandoStatus = false;

int ultimoS1 = HIGH;
int ultimoS2 = HIGH;
int ultimoEstadoBotao = HIGH;
const unsigned long DEBOUNCE_ENCODER = 5;
unsigned long ultimoMovimento = 0;
unsigned long ultimoClique = 0;
unsigned long tempoTela = 10;
const unsigned long DEBOUNCE = 250;

float temperatura = 0;
unsigned long ultimaLeituraTemp = 0;
unsigned long retornoStatus = 0;
