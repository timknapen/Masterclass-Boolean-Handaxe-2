#ifndef _GAMETIME_PINS_1M_HOUSES_
#define _GAMETIME_PINS_1M_HOUSES_

// #define USE_POTS

/*** 1 Million Houses hardware */
#warning ONE MILLION HOUSES

// Colors - display
#define BLACK 0
#define WHITE 1
#define GRAY 2

// screen dimensions
const int height = 240;
const int width = 400;

// SPI pin GAMETIME PROTO 1
// #define SCK 17
// #define MOSI 18
// #define CS 8

// SHARP MEMORY LCD display pins
#define PIN_LCD_CLK 17
#define PIN_LCD_DI 18 // MOSI
#define PIN_LCD_CS 8

#define NUMBUTTONS 6

// PIN DEFINITIONS
#define BTN_PIN_BOOT 0
#define BTN_PIN_A 41
#define BTN_PIN_B 2
#define BTN_PIN_X 1
#define BTN_PIN_Y 42
#define BTN_PIN_Z 40

// PIN DEFINITIONS AUDIO
// numbers are GPIO
#define PIN_I2S_DIN -1  // 3
#define PIN_I2S_DOUT -1 // 10

#define PIN_I2S_SCLK -1
#define PIN_SGTL_MCLK -1
#define PIN_I2S_LRCLK -1

// PIN DEFINITIONS SD CARD
#define PIN_SD_DAT1 11
#define PIN_SD_DAT0 12
#define PIN_SD_DAT2 48
#define PIN_SD_DAT3_CS 47
#define PIN_SD_CMD 21
#define PIN_SD_CD 14 // Card Detect switch
#define PIN_SD_CLK 13

// GameTime I2C
#define PIN_SCL 16
#define PIN_SDA 15

// ENABLE PCA9685
#define PIN_PCA_ENABLE 38

// POTS / ANALOG
#define NUMPOTS 6

#define PIN_BAT_MEASURE 3
#define PIN_POT_A 4
#define PIN_POT_B 5
#define PIN_POT_C 6
#define PIN_POT_D 7
#define PIN_POT_E 10
#define PIN_POT_F 9

#endif