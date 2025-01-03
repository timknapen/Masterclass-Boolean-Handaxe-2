#ifndef _PINS_PINS_H_
#define _PINS_PINS_H_

// I2C pins
#define PIN_SCL 16
#define PIN_SDA 17

// AT42QT2120 capacitive touch sensor interrupt pin
#define CAPTOUCH_IRQ_PIN 18

// Colors
#define BLACK 0
#define WHITE 1
#define GRAY 2

// screen dimensions
const int height = 240;
const int width = 400;

// RGB LED on board
#define PIN_RGB_LED 38

// SHARP MEMORY LCD display pins
#define PIN_LCD_CLK 11
#define PIN_LCD_DI 12
#define PIN_LCD_CS 13

// input pin
#define PIN_BTN 21

#endif