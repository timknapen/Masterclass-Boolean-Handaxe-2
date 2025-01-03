#include "housesProto.h"
#include "pins_1M_houses.h"
#include <Adafruit_GFX.h>
// #include <Adafruit_NeoPixel.h>
#include <Adafruit_SharpMem.h>
#include <Arduino.h>
#include <Wire.h>

// the ESP32-S3 dev board has one RGB LED aka NeoPixel
// Adafruit_NeoPixel pixels(1, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);

// Timing for drawing frames
unsigned long lastFrame = 0;
// A little time offset we can randomize, so we start in a different place each
// time
unsigned long timeOffset = 0;

// SHARP display named display
Adafruit_SharpMem display =
    Adafruit_SharpMem(&SPI, PIN_LCD_CS, width, height, 8000000);

// postions of waves
float ya, yb; // left and right of wave 1
float yc, yd; // left and right of wave 2
// our buoy is just a circle
float buox = width / 2;
float buoy = height; // buoy already ends with a y.
float buoyRadius = 40;

// setup the display
void setupDisplay() {
  SPI.begin(PIN_LCD_CLK, -1, PIN_LCD_DI, PIN_LCD_CS);
  display.begin();
  display.setRotation(2);
  display.clearDisplay();
}

bool bAnimate = true;

//--------------------------------------------------
void drawCounter() {
  // draw something moving to show slow down in framerate
  unsigned long now = millis();
  float a = (float)now / 100.0f;
  int x = 20 + 10 * cos(a);
  int y = 20 + 10 * sin(a);
  display.fillCircle(x, y, 10, BLACK);
}

//--------------------------------------------------
void drawScene() {
  // Draw our simple waves scene.
  // first calculate the positions of our two waves and the buoy
  // Our only input is time
  unsigned long now = millis();

  // random starting point
  now += timeOffset % 10000;

  // Calculate positions
  // Wave 1 positions - far
  ya = height / 2 + 30 * getPotVal(0) * sin((float)now / 1000.0f);
  yb = height / 2 + 30 * getPotVal(1) * sin(PI + (float)now / 500.0f);
  // Wave 2 positions - near
  yc = height / 2 + 20 + 40 * getPotVal(2) * sin((float)now / 666.0f);
  yd = height / 2 + 20 + 40 * getPotVal(3) * sin(PI + (float)now / 330.0f);
  // Buoy position
  // float with a slight delay behind wave 2
  buox += (yd - yc) / 20.0f; // move left and right with our wave
  // keep the buoy inside of the screen
  if (buox < buoyRadius) {
    buox = buoyRadius;
  }
  if (buox > width - buoyRadius) {
    buox = width - buoyRadius;
  }
  // for the y position, we'll get there in two steps
  // y position based on left right pos between extremes of wave 2 (yc, yd)
  float newBuoy = (yc + (yd - yc) * buox / width);
  newBuoy -= 5;                     // sit a little higher up on the water
  buoy += (newBuoy - buoy) / 15.0f; // zeno

  // Actual drawing starts here
  // and goes from background to foreground

  // The Sky
  // display.fillRect(0, 0, width, height, GRAY);

  // Horizon - super far away
  display.fillRect(0, height / 2 + 10, width, height / 2 + 10, BLACK);

  // Wave 1 - far away
  display.fillTriangle(0, ya, width, yb, 0, height, BLACK);
  display.fillTriangle(0, height, width, yb, width, height, BLACK);

  // Buoy - right behind wave 2
  display.fillCircle(buox, buoy, buoyRadius, WHITE);
  // display.drawFatLine(buox, buoy, buox + 20, buoy - 80, 6, WHITE);

  // Wave 2 - near
  display.fillTriangle(0, yc, width, yd, width, height, BLACK);
  display.fillTriangle(0, yc, width, height, 0, height, BLACK);
}

//--------------------------------------------------
void draw() {
  if (!bAnimate) {
    return;
  }
  // Do drawing here
  // this function is called only 60 times/ sec aka 60fps
  display.clearDisplayBuffer(); // clear our display buffer

  // drawCounter(); // draw something that moves
  // Draw horizontal touches!
  // drawHorizontalTouches();

  // Draw Circular touches
  // drawCircularTouches(); // draw the touches we see.

  drawScene();

  display.refresh(); // actually sends it to the display
}

//--------------------------------------------------
void setup() {
  Serial.begin((57600));          // start a serial port at 57600 BAUD
  delay(1000);                    // wait 100ms
  Serial.println(" A Wild Sea "); // send a message over serial

  setupDisplay();

  // setup pca
  pca_setup();
  // pca_demo();

  setupPots();
  setupButtons();

  // Turn our RGB led off
  // pixels.begin();
  // pixels.setPixelColor(0, 0, 0, 0);
  // pixels.show();

  // Setup the display and show feedback
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(5, 5); // where will we draw?
  display.println(" The display works");
  display.refresh(); // actually sends it to the display
  delay(500);

  // set our time offset to start in a random spot each time we
  timeOffset = random();
  Serial.print("starting at ");
  Serial.println(timeOffset);

  // setup touch sensor and show feedback
  // setupTouchSensor();
  // display.clearDisplay();
  // display.setCursor(5, 5);
  // display.println(" Setup touch");
  // display.refresh();
  // delay(100);

  display.clearDisplay();
  pca_enable();
}

//--------------------------------------------------
void loop() {

  unsigned long now = millis();      // get the current time
  if (now > lastFrame + 1000 / 60) { // do this every 60fps
    lastFrame = now;
    updatePots();
    // read the button
    bAnimate = true;
    if (digitalRead(btnPins[0]) == LOW) {
      bAnimate = false;
    }
    // updateTouchSensor(); // get data from the capacitive touch sensor
    draw(); // draw to screen
  }
}
