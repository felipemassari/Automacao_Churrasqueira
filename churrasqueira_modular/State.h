#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

extern int tempDHT;
extern int umidDHT;

extern bool luz;
extern bool exaustor;
extern bool soprador;

extern const char* menu[];
extern const char* menuConfig[];

extern const int MENUCONFIG_TOTAL;
extern const int MENU_TOTAL;
extern int menuAtual;

extern bool editandoOpcao;
extern bool estadoTemporario;

extern int ultimoS1;
extern int ultimoEstadoBotao;
extern unsigned long ultimoClique;
extern const unsigned long DEBOUNCE;

extern float temperatura;
extern unsigned long ultimaLeituraTemp;
extern unsigned long retornoStatus;

#endif
