#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

extern int tempDHT;
extern int umidDHT;
extern int setTempExaustor;
extern int setTempSoprador;

extern bool luz;
extern bool exaustor;
extern bool soprador;
extern bool ledMureta;
extern bool dentroConfig;

extern const char* menu[];
extern const char* menuConfig[];

extern const int MENU_CONFIG_TOTAL;
extern const int MENU_TOTAL;
extern int menuAtual;
extern int menuConfigAtual;

extern bool editandoOpcao;
extern bool estadoTemporario;
extern bool modoAutomatico;
extern bool estadoConfigTemp;
extern bool aguardandoStatus;

extern int ultimoS1;
extern int ultimoS2;
extern int valorConfigTemp;
extern int ultimoEstadoBotao;
extern unsigned long ultimoMovimento;
extern unsigned long ultimoClique;
extern const unsigned long DEBOUNCE;
extern const unsigned long DEBOUNCE_ENCODER;

extern float temperatura;
extern unsigned long ultimaLeituraTemp;
extern unsigned long tempoTela;
extern unsigned long retornoStatus;

#endif
