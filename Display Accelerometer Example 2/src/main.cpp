#include "pins.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_FXOS8700.h>
// #include <Adafruit_Sensor.h>

// accelerometer FXOS8700
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

// screen dimensions
const int height = 240;
const int width = 400;

// ball position and radius
float ballX = width / 2;
float ballY = height / 2;
int ballR = 10;

// Timing
unsigned long lastFrame = 0;

// SHARP display named display
Adafruit_SharpMem display =
    Adafruit_SharpMem(&SPI, PIN_LCD_CS, width, height, 4000000);

// setup the display
void setupDisplay() {
  SPI.begin(PIN_LCD_CLK, -1, PIN_LCD_DI, PIN_LCD_CS);
  display.begin();
  display.setRotation(0);
  display.clearDisplay();
}

// accelerometer
int aX, aY, aZ;

// setup accelerometer
//--------------------------------------------------
void setupAccelerometer() {
  Wire.begin(PIN_SDA, PIN_SCL, 100000UL); // set the I2C pins first

  if (!accelmag.begin()) {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while (1)
      ;
  }
}

//--------------------------------------------------
void updateAccelerometer() {
  sensors_event_t aevent, mevent;

  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);

  /* Display the accel results (acceleration is measured in m/s^2) */
  Serial.print("A ");
  Serial.print("X: ");
  Serial.print(aevent.acceleration.x, 4);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(aevent.acceleration.y, 4);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(aevent.acceleration.z, 4);
  Serial.print("  ");
  Serial.println("m/s^2");

  /* Display the mag results (mag data is in uTesla) */
  Serial.print("M ");
  Serial.print("X: ");
  Serial.print(mevent.magnetic.x, 1);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(mevent.magnetic.y, 1);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(mevent.magnetic.z, 1);
  Serial.print("  ");
  Serial.println("uT");

  Serial.println("");
}

//--------------------------------------------------
void setup() {
  Serial.begin((57600)); // start a serial port at 57 600 BAUD
  delay(1000);           // wait one second aka 1000ms
  Serial.println("Display + Accelerometer Test"); // send a message over serial

  setupDisplay();
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(5, 5);
  display.println(" Display works");
  display.refresh(); // actually sends it to the display
  delay(1000);

  setupAccelerometer();
  display.clearDisplay();
  display.setCursor(5, 5);
  display.println(" Accelerometer setup");
  display.refresh();
  delay(1000);

  display.clearDisplay();
}

#define TXT_LEN 64
char txt[TXT_LEN];

//--------------------------------------------------
void loop() {

  unsigned long now = millis(); // get the current time

  if (now > lastFrame + 1000 / 60) { // do this every 60fps
    lastFrame = now;

    updateAccelerometer();

    display.clearDisplayBuffer();
    display.setCursor(0, 5);
    // draw XYZ accelerometer

    // snprintf(txt, TXT_LEN, " X: %d", aX);
    // display.println(txt);
    // snprintf(txt, TXT_LEN, " Y: %d", aY);
    // display.println(txt);
    // snprintf(txt, TXT_LEN, " Z: %d", aZ);
    // display.println(txt);

    // draw graphs
    // display.fillRect(width / 2, height / 2, aX / 10, 10, BLACK);
    // display.fillRect(width / 2, height / 2 + 15, aY / 10, 10, BLACK);
    // display.fillRect(width / 2, height / 2 + 30, aZ / 10, 10, BLACK);

    // draw position from center
    display.drawFatLine(width / 2, height / 2, width / 2 + aX / 10,
                        height / 2 + aY / 10, 2, BLACK);

    // the bouncy ball
    //  ballX = width / 2 + 100 * cos((float)now / width);
    //  ballY = height / 2 + 100 * sin((float)now / width);
    ballX += (float)aX / 100;
    ballY += (float)aY / 100;
    // keep within bounds
    if (ballX < ballR) {
      ballX = ballR;
    }
    if (ballX > width - ballR) {
      ballX = width - ballR;
    }
    if (ballY < ballR) {
      ballY = ballR;
    }
    if (ballY > height - ballR) {
      ballY = height - ballR;
    }

    display.fillCircle(ballX, ballY, ballR, BLACK);

    display.refresh(); // actually sends it to the display
  }
}
