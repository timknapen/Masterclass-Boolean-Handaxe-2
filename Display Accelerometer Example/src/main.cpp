#include "pins.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// accelerometer
Adafruit_MMA8451 mma = Adafruit_MMA8451();

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
  Wire.begin(PIN_SDA, PIN_SCL, 50000UL); // set the I2C pins first

  if (!mma.begin()) {
    Serial.println("Couldnt start");
    while (1)
      ;
  }
  Serial.println("MMA8451 found!");

  mma.setRange(MMA8451_RANGE_2_G);

  Serial.print("Range = ");
  Serial.print(2 << mma.getRange());
  Serial.println("G");
}

//--------------------------------------------------
void updateAccelerometer() {

  mma.read();
  aY = -mma.x;
  aX = -mma.y;
  aZ = mma.z;

  // Serial.print("X:\t");
  // Serial.print(mma.x);
  // Serial.print("\tY:\t");
  // Serial.print(mma.y);
  // Serial.print("\tZ:\t");
  // Serial.print(mma.z);
  // Serial.println();

  /* Get a new sensor event */
  /*
  sensors_event_t event;
  mma.getEvent(&event);

  // Display the results (acceleration is measured in m/s^2)
  Serial.print("X: \t");
  Serial.print(event.acceleration.x);
  Serial.print("\t");
  Serial.print("Y: \t");
  Serial.print(event.acceleration.y);
  Serial.print("\t");
  Serial.print("Z: \t");
  Serial.print(event.acceleration.z);
  Serial.print("\t");
  Serial.println("m/s^2 ");

  // Get the orientation of the sensor
  uint8_t o = mma.getOrientation();
  //*/
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
