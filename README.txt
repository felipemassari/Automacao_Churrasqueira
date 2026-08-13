CHURRASQUEIRA AUTOMATIZADA - VERSÃO MODULAR
==============================================

BASE
----
Este projeto foi separado em arquivos .h/.cpp mantendo a lógica
do código final fornecido.

ARQUIVOS
--------
churrasqueira_modular.ino
    setup() e loop().

Config.h
    Pinos, endereços OLED e posições dos elementos do Display 1.

Icons.h
    Ícones:
      20x20: soprador, luz, exaustor
      15x15: temperatura, umidade

State.h / State.cpp
    Estados e variáveis globais.

Display.h / Display.cpp
    Desenho dos dois OLEDs.

Encoder.h / Encoder.cpp
    Encoder, botão e menu.

Temperature.h / Temperature.cpp
    MAX6675.

Connectivity.h / Connectivity.cpp
    Preparação para Wi-Fi/MQTT. DESATIVADO.

DISPLAY 1
---------
Topo:
  temperatura DHT + valor
  umidade DHT + valor

Meio:
  BBQ + temperatura da churrasqueira

Parte inferior:
  soprador / luz / exaustor

POSIÇÕES
--------
Todas estão em Config.h, facilitando ajustes sem mexer nas
funções do display.

DHT11
-----
Ainda não está instalado nesta versão.
Valores simulados:
  temperatura = 25 C
  umidade = 60 %

Eles estão em State.cpp.

WIFI / MQTT
-----------
Já existe a estrutura, mas está DESATIVADA.

Quando for utilizar:
  1. configurar SSID/senha;
  2. configurar broker MQTT;
  3. definir tópicos;
  4. descomentar o código marcado como FUTURO.

RELÉS
-----
Os relés ainda NÃO foram incorporados nesta etapa porque o
código final enviado não possui os pinos/rotinas dos relés.
A próxima etapa pode ligar os estados do menu aos 4 relés.

OBSERVAÇÃO
----------
O código foi mantido próximo ao original para facilitar testes
e futuras alterações.
