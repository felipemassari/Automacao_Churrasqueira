#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// I2C
// ============================================================

#define SDA_PIN 3
#define SCL_PIN 1

// ============================================================
// OLED
// ============================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define OLED1_ADDRESS 0x3D
#define OLED2_ADDRESS 0x3C

// ============================================================
// MAX6675
// ============================================================

#define MAX_SCK 0
#define MAX_CS  2
#define MAX_SO  21

// ============================================================
// ENCODER
// ============================================================

#define ENC_S1  10
#define ENC_S2  8
#define ENC_KEY 6

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
#define POS_BBQ_Y        35

#define POS_TEMP_BBQ_X   45
#define POS_TEMP_BBQ_Y   40

#define POS_SOPRADOR_X   5
#define POS_SOPRADOR_Y   44

#define POS_LUZ_X        54
#define POS_LUZ_Y        45

#define POS_EXAUSTOR_X   103
#define POS_EXAUSTOR_Y   45

// Reservado para DHT11 futuro
#define DHT_PIN 4

#endif
