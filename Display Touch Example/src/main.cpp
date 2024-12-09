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
void setup() {
  Serial.begin((57600));                  // start a serial port at 57600 BAUD
  delay(1000);                            // wait one second aka 1000ms
  Serial.println("Display + Touch Test"); // send a message over serial

  setupDisplay();

  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(5, 5);
  display.println(" Display works!!!!");
  display.refresh(); // actually sends it to the display
  delay(1000);

  setupTouchSensor();
  display.clearDisplay();
  display.setCursor(5, 5);
  display.println(" Setup touch");
  display.refresh();
  delay(1000);

  display.clearDisplay();
}

//--------------------------------------------------
void loop() {

  unsigned long now = millis(); // get the current time

  if (now > lastFrame + 1000 / 60) { // do this every 60fps
    lastFrame = now;

    updateTouchSensor();

    display.clearDisplayBuffer();
    // the bouncy ball
    int x = (now / 10) % 400;
    int y = height / 2 + 100 * sin((float)now / 400);
    display.fillCircle(x, y, 10, BLACK);

    // draw touches!
    for (uint8_t i = 0; i < 12; i++) {
      // it if *is* touched and *wasnt* touched before, alert!
      if (currtouched & _BV(i)) {
        int w = width / 12;
        x = i * w;
        y = height - 30;
        display.fillRect(x, y, w, w, BLACK);
      }
    }

    display.refresh(); // actually sends it to the display
  }
}
