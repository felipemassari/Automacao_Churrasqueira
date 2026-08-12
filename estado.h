#ifndef ESTADO_H
#define ESTADO_H

// Estados dos equipamentos.
// Atualmente são apenas simulados.
// Quando os relés forem adicionados, este será o ponto
// central para ligar/desligar cada saída.

extern bool luz;
extern bool exaustor;
extern bool soprador;
extern bool led;

// Temperatura medida pelo MAX6675.
extern float temperatura;

#endif
