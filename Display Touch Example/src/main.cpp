#include "Adafruit_MPR121.h"
#include "pins.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Arduino.h>
#include <Wire.h>

// screen dimensions
const int height = 240;
const int width = 400;

// Timing
unsigned long lastFrame = 0;

// SHARP display named display
Adafruit_SharpMem display =
    Adafruit_SharpMem(&SPI, PIN_LCD_CS, width, height, 8000000);

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

// setup the display
void setupDisplay() {
  SPI.begin(PIN_LCD_CLK, -1, PIN_LCD_DI, PIN_LCD_CS);
  display.begin();
  display.setRotation(0);
  display.clearDisplay();
}

// setup MPR121 touch sensor
//--------------------------------------------------
void setupTouchSensor() {
  Wire.begin(PIN_SDA, PIN_SCL); // set the I2C pins first

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
      ;
  } else {
    Serial.println("MPR121 found!");
  }
}

//--------------------------------------------------
void updateTouchSensor() {
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i))) {
      Serial.print(i);
      Serial.println(" released");
    }
  }
}

//--------------------------------------------------
void drawHorizontalTouches() {
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if (currtouched & _BV(i)) {
      int w = width / 12;
      int x = i * w;
      int y = height - 30;
      display.fillRect(x, y, w, w, BLACK);
    }
  }
}

//--------------------------------------------------
void drawCircularTouches() {
  // sequence of the pads going from 1 o'clock, clockwise
  // when the connector is at the top
  // 7 10 11 9 8 6 5 3 2 0 1 4
  int touchPads[12] = {7, 10, 11, 9, 8, 6, 5, 3, 2, 0, 1, 4};
  int cx = width / 2;
  int cy = height / 2;
  float w = 90;
  float r = 20;
  float angle = 0;
  float angleOffset = PI / 2 + TWO_PI / 24;

  for (uint8_t i = 0; i < 12; i++) {
    angle = (float)i * TWO_PI / 12;
    int x = cx + w * cos(angleOffset + angle);
    int y = cy + w * sin(angleOffset + angle);
    if (currtouched & _BV(touchPads[i])) {
      display.fillCircle(x, y, r, BLACK);
    } else {
      display.fillCircle(x, y, r, GRAY);
    }
  }
}

//--------------------------------------------------
void drawCounter() {
  // the bouncy ball
  unsigned long now = millis();
  float a = (float)now / 400;
  int x = 20 + 10 * cos(a);
  int y = 20 + 10 * sin(a);
  display.fillCircle(x, y, 5, BLACK);
}

//--------------------------------------------------
void setup() {
  Serial.begin((57600));                  // start a serial port at 57600 BAUD
  delay(100);                             // wait 100ms
  Serial.println("Display + Touch Test"); // send a message over serial

  setupDisplay();

  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(5, 5);
  display.println(" Display works!!!!");
  display.refresh(); // actually sends it to the display
  delay(500);

  setupTouchSensor();
  display.clearDisplay();
  display.setCursor(5, 5);
  display.println(" Setup touch");
  display.refresh();
  delay(100);

  display.clearDisplay();
}

//--------------------------------------------------
void loop() {

  unsigned long now = millis(); // get the current time

  if (now > lastFrame + 1000 / 60) { // do this every 60fps
    lastFrame = now;

    updateTouchSensor(); // get data from the capacitive touch sensor

    display.clearDisplayBuffer(); // clear our display buffer

    drawCounter(); // draw something that moves
    // Draw horizontal touches!
    // drawHorizontalTouches();

    // Draw Circular touches
    drawCircularTouches(); // draw the touches we see.

    display.refresh(); // actually sends it to the display
  }
}
