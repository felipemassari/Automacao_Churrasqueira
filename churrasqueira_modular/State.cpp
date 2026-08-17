#include "State.h"

// DHT11 ainda não instalado: valores simulados
int tempDHT = 25;
int umidDHT = 60;

bool luz = false;
bool exaustor = false;
bool soprador = false;
bool ledMureta = false;

bool modoAutomatico = true;

int setTempExaustor = 100;
int setTempSoprador = 200;

const char* menu[] = {
  "Luz",
  "Exaustor",
  "Soprador",
  "Config"
};

const char* menuConfig[] = {
  "Modo Automatico",
  "Tempo de Tela",
  "setTemp Exaustor",
  "setTemp Soprador",
  "ledMureta"
};

const int MENU_TOTAL = 4;
const int MENUCONFIG_TOTAL = 5;
int menuAtual = 0;

bool editandoOpcao = false;
bool estadoTemporario = false;

int ultimoS1 = HIGH;
int ultimoEstadoBotao = HIGH;
unsigned long ultimoClique = 0;
unsigned long tempoTela = 10000;
const unsigned long DEBOUNCE = 250;

float temperatura = 0;
unsigned long ultimaLeituraTemp = 0;
unsigned long retornoStatus = 0;
