# Churrasqueira Automatizada — código modular

Este projeto é a versão organizada do código-base que estava funcionando no ESP32-C3.

A organização foi feita **sem mudar a lógica principal do programa**. A ideia é que, a partir daqui, cada parte possa crescer sem transformar o `.ino` em um arquivo enorme.

## Hardware atual

- ESP32-C3 SuperMini
- 2 OLED SSD1306 128x64 via I2C
- Encoder com S1, S2 e KEY
- MAX6675 para temperatura da churrasqueira
- Relés ainda não acionados pelo software
- DHT11 ainda não instalado

## Pinagem

### I2C

| Função | GPIO |
|---|---:|
| SDA | 3 |
| SCL | 1 |

### OLED

| Display | Endereço I2C |
|---|---|
| OLED 1 | `0x3D` |
| OLED 2 | `0x3C` |

O scanner I2C continua no `setup()` para facilitar diagnóstico.

### MAX6675

| Função | GPIO |
|---|---:|
| SCK | 0 |
| CS | 2 |
| SO | 21 |

### Encoder

| Pino do encoder | Função | GPIO |
|---|---|---:|
| S1 | CLK | 10 |
| S2 | DT | 8 |
| KEY | Botão | 6 |

## Como o programa funciona

### Display 1

Mostra:

- título `CHURRASQUEIRA`
- temperatura lida pelo MAX6675
- `C`
- quatro ícones na parte inferior:
  - Luz
  - Exaustor
  - Soprador
  - LED

Os ícones aparecem somente quando o respectivo estado está ligado.

### Display 2

O encoder navega entre:

1. Luz
2. Exaustor
3. Soprador
4. LED
5. Config

Nos quatro primeiros itens, o display mostra `ON` ou `OFF`.

Em `Config`, aparece uma tela provisória.

### Encoder

Girando:

- horário → próximo item
- anti-horário → item anterior

Clicando:

- Luz → alterna Luz
- Exaustor → alterna Exaustor
- Soprador → alterna Soprador
- LED → alterna LED
- Config → abre a tela provisória

O botão usa detecção de transição e debounce. Portanto, **segurar o botão não fica executando vários cliques**.

## Organização dos arquivos

### `Churrasqueira.ino`

É o programa principal.

Só coordena:

- inicialização
- leitura do encoder
- leitura da temperatura
- atualização do sistema

### `config.h`

Concentra as configurações físicas:

- GPIOs
- endereços I2C
- tamanho dos OLEDs
- debounce

Se mudar um pino, este é o primeiro arquivo a consultar.

### `estado.h / estado.cpp`

Guarda os estados atuais:

```text
luz
exaustor
soprador
led
temperatura
```

Hoje os estados são apenas variáveis.

Quando os relés forem adicionados, a lógica de acionamento pode ser colocada aqui ou em um módulo separado de relés.

### `menu.h / menu.cpp`

Contém:

- nomes do menu
- quantidade de itens
- posição atual
- função que informa o estado do item selecionado

### `displays.h / displays.cpp`

Tudo relacionado aos OLEDs:

- criação dos dois displays
- inicialização
- Display 1
- Display 2
- tela Config

### `icones.h / icones.cpp`

Contém somente os desenhos dos ícones.

Cada função recebe:

```cpp
display
x
y
ligado
```

Assim fica fácil mudar a posição ou o desenho sem mexer na lógica do menu.

### `encoder.h / encoder.cpp`

Responsável por:

- S1
- S2
- KEY
- rotação
- botão
- debounce
- proteção contra botão segurado

### `temperatura.h / temperatura.cpp`

Responsável pelo MAX6675.

A leitura é feita aproximadamente a cada 1 segundo.

No futuro este módulo pode receber também o DHT11.

### `i2c.h / i2c.cpp`

Responsável por:

- iniciar o barramento I2C
- executar o scanner I2C

## Bibliotecas

O projeto utiliza:

- `Wire`
- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `MAX6675`

Todas devem estar instaladas no Arduino IDE.

## Próximos módulos

A estrutura já está preparada para crescer.

### Relés

O próximo módulo pode ser:

```text
reles.h
reles.cpp
```

Ele poderá receber comandos como:

```cpp
ligarLuz();
desligarLuz();

ligarExaustor();
desligarExaustor();

ligarSoprador();
desligarSoprador();

ligarLED();
desligarLED();
```

A interface do menu não precisará conhecer diretamente os GPIOs dos relés.

### DHT11

Pode ser adicionado em:

```text
temperatura.h
temperatura.cpp
```

ou, se ficar mais conveniente:

```text
dht.h
dht.cpp
```

Os valores de temperatura/umidade da varanda poderão então ser enviados ao Display 1.

### Config

A tela `Config` está propositalmente provisória.

Ela pode posteriormente receber:

- temperatura alvo
- temporizadores
- modo automático
- ajustes do soprador
- ajustes do exaustor
- configurações do sistema

## Fluxo geral

```text
                    ESP32-C3
                       |
        +--------------+--------------+
        |              |              |
       I2C           Encoder        MAX6675
        |              |              |
   +----+----+      S1/S2/KEY      Temperatura
   |         |
 OLED 1    OLED 2
   |         |
   |       Menu
   |
Temperatura
+ estados
+ ícones

Encoder
   |
   +--> gira --> muda menu
   |
   +--> clique --> muda estado
                     |
                     +--> futuramente relé
```

## Regra importante para futuras alterações

A ideia desta estrutura é:

**não colocar tudo no `Churrasqueira.ino`.**

Se a alteração for de:

- pino → `config.h`
- menu → `menu.cpp`
- display → `displays.cpp`
- ícone → `icones.cpp`
- encoder → `encoder.cpp`
- temperatura → `temperatura.cpp`
- estado dos equipamentos → `estado.cpp`
- relés → futuro `reles.cpp`

Isso mantém o projeto fácil de testar e evita voltar ao código único de centenas de linhas.


## Wi-Fi e MQTT

O projeto possui módulos separados:

```text
wifi.h
wifi.cpp
mqtt.h
mqtt.cpp
```

### Configuração

Todos os parâmetros ficam no `config.h`:

```cpp
#define WIFI_SSID "SEU_WIFI"
#define WIFI_PASSWORD "SUA_SENHA"

#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883

#define MQTT_USER ""
#define MQTT_PASSWORD ""
```

Substitua os valores pelos dados da sua rede e do seu broker MQTT.

### Publicações

O ESP32 publica os estados da churrasqueira:

```text
churrasqueira/temperatura
churrasqueira/luz
churrasqueira/exaustor
churrasqueira/soprador
```

Os estados são publicados como:

```text
ON
OFF
```

A temperatura é publicada numericamente em graus Celsius.

As mensagens de estado são retidas (`retain`), para que o Home Assistant ou outro cliente receba o último estado assim que se inscrever.

### Comandos

O ESP32 aceita comandos nestes tópicos:

```text
churrasqueira/cmd/luz
churrasqueira/cmd/exaustor
churrasqueira/cmd/soprador
```

Podem ser enviados:

```text
ON
OFF
```

Também são aceitos:

```text
1
0
LIGADO
DESLIGADO
TRUE
```

### LED / luminária

A luminária LED **não faz parte da automação da churrasqueira**.

Existe apenas a definição:

```text
luminaria/led
```

para uso futuro do comando MQTT da luminária. Ela não altera `luz`, `exaustor` ou `soprador` e não aparece no menu da churrasqueira.

Quando o módulo do relé da luminária for implementado, ele poderá simplesmente assinar esse tópico.

### Funcionamento quando o Wi-Fi/MQTT cai

A conexão MQTT não deve impedir o funcionamento local.

Se o Wi-Fi ou broker estiver indisponível:

- o ESP32 continua executando o programa;
- encoder continua funcionando;
- displays continuam funcionando;
- temperatura continua sendo lida;
- o MQTT tenta reconectar nos ciclos seguintes.

