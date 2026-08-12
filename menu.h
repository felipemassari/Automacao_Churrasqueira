#ifndef MENU_H
#define MENU_H

// Itens navegáveis pelo encoder.
extern const char* menu[];
extern const int MENU_TOTAL;

extern int menuAtual;

// Retorna o estado real do item atualmente selecionado.
// Para Config, retorna false.
bool estadoMenuAtual();

#endif
