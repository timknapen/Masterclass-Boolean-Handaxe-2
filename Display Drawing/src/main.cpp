#include "pins.h"
#include <Adafruit_GFX.h>
// #include <Adafruit_NeoPixel.h>
#include <Adafruit_SharpMem.h>
#include <Arduino.h>

// screen dimensions
const int height = 240;
const int width = 400;

// position of my character
float x = width / 2;
float y = height / 2;

// our velocity
float vx = 1;
float vy = 1;

// Timing
unsigned long lastFrame = 0;

// SHARP display named display
Adafruit_SharpMem display =
    Adafruit_SharpMem(&SPI, PIN_LCD_CS, width, height, 8000000);

// setup the display
void setupDisplay() {
  SPI.begin(PIN_LCD_CLK, -1, PIN_LCD_DI, PIN_LCD_CS);
  display.begin();
  display.setRotation(0);
  display.clearDisplay();
}

//--------------------------------------------------
void setup() {
  Serial.begin((57600));             // start a serial port at 57600 BAUD
  delay(100);                        // wait one second aka 1000ms
  Serial.println(" A Display Test"); // send a message over serial

  setupDisplay();

  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(5, 5);
  display.println(" IT WORKS!!!!");
  display.refresh(); // actually sends it to the display

  delay(100);
  display.clearDisplayBuffer();
}

//--------------------------------------------------
void loop() {

  unsigned long now = millis();      // get the current time
  if (now > lastFrame + 1000 / 60) { // do this every 60fps
    lastFrame = now;
    x = x + vx;
    y = y + vy;

    float maxSpeed = 2;
    vx = vx + (float)random(-10, 10) / 100;
    vy = vy + (float)random(-10, 10) / 100;

    if (vx > maxSpeed) {
      vx = maxSpeed;
    }
    if (vx < -maxSpeed) {
      vx = -maxSpeed;
    }
    if (vy > maxSpeed) {
      vy = maxSpeed;
    }
    if (vy < -maxSpeed) {
      vy = -maxSpeed;
    }

    int radius = 20;
    if (x + radius > width) { // ball over right edge
      vx = -vx;
      x = width - radius;
    }
    if (x < 0 + radius) { // ball over left edge
      vx = -vx;
      x = 0 + radius;
    }
    if (y > height - radius) { // ball over bottom of screen
      vy = -vy;
      y = height - radius;
    }
    if (y < radius) { // ball over top of screen
      vy = -vy;
      y = radius;
    }

    // display.clearDisplayBuffer();
    display.fillCircle(x, y, radius, BLACK);
    display.fillCircle(x, y, radius / 2, WHITE);

    display.refresh(); // actually sends it to the display
  }
}
