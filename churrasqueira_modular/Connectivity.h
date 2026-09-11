#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

// ============================================================
// WIFI / MQTT
// ============================================================

void iniciarConectividade();

void conectarMQTT();

void publicarDiscovery();

void callbackMQTT(char* topic, byte* payload, unsigned int length);

void sincronizarMQTT();

void publicarMQTT();

void processarConectividade();

extern bool wifiConnected;

#endif