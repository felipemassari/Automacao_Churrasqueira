#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// LED CONEXAO
// ============================================================
#define LED_CONNECT 21

// ============================================================
// I2C
// ============================================================
#define SCL_PIN 0
#define SDA_PIN 1

// ============================================================
// ENCODER
// ============================================================
#define ENC_KEY 2
#define ENC_S1  3
#define ENC_S2  4

// ============================================================
// MAX6675
// ============================================================
#define MAX_SCK 5
#define MAX_CS  6
#define MAX_SO  7

// ============================================================
// RELÉS
// ============================================================
#define RELE_LUZ       8
#define RELE_EXAUSTOR  9
#define RELE_SOPRADOR  10
#define RELE_LED       20

// ============================================================
// OLED
// ============================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define OLED1_ADDRESS 0x3D
#define OLED2_ADDRESS 0x3C


// ============================================================
// POSIÇÕES - DISPLAY 1
// ============================================================
#define POS_TEMP_ICON_X  2
#define POS_TEMP_ICON_Y  0

#define POS_TEMP_DHT_X   20
#define POS_TEMP_DHT_Y   14

#define POS_UMID_ICON_X  70
#define POS_UMID_ICON_Y  0

#define POS_UMID_DHT_X   88
#define POS_UMID_DHT_Y   14

#define POS_BBQ_X        1
#define POS_BBQ_Y        37

#define POS_TEMP_BBQ_X   55
#define POS_TEMP_BBQ_Y   43

#define POS_SOPRADOR_X   5
#define POS_SOPRADOR_Y   44

#define POS_LUZ_X        54
#define POS_LUZ_Y        46

#define POS_EXAUSTOR_X   103
#define POS_EXAUSTOR_Y   43

#endif
