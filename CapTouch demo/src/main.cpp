#include "pins.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SharpMem.h>
#include <Arduino.h>
#include <Wire.h>

#include "TKPoint.h"
#include <AT42QT2120.h>

// the ESP32-S3 dev board has one RGB LED aka NeoPixel
Adafruit_NeoPixel pixels(1, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);

// Timing for drawing frames
unsigned long lastFrame = 0;

// SHARP display named display
Adafruit_SharpMem display =
    Adafruit_SharpMem(&SPI, PIN_LCD_CS, width, height, 8000000);

// AT42QT2120 Capacitive touch sensor
const int RESET_DELAY = 1000;
const int CALIBRATION_LOOP_DELAY = 50;
AT42QT2120 touch_sensor(Wire, CAPTOUCH_IRQ_PIN);

// interrupt callback
volatile bool change_line_activated = false;
void changeLineActivated() { change_line_activated = true; }

// store touch data
uint32_t keys = 0x00;
float wheelAngle = 0; // wheelangle
TKPoint arrow;

//--------------------------------------------------
void setupTouchSensor() {
  Wire.begin(PIN_SDA, PIN_SCL);
  pinMode(CAPTOUCH_IRQ_PIN, INPUT_PULLUP);
  touch_sensor.begin();

  Serial.println("AT42QT2120 Reseting ...");
  touch_sensor.reset();
  delay(RESET_DELAY);

  Serial.println("AT42QT2120 triggerCalibration");
  touch_sensor.triggerCalibration();
  delay(CALIBRATION_LOOP_DELAY);
  while (touch_sensor.calibrating()) {
    Serial.println("AT42QT2120 calibrating...");
    delay(CALIBRATION_LOOP_DELAY);
  }
  Serial.println("AT42QT2120 finished calibrating");

  touch_sensor.attachChangeCallback(changeLineActivated);

  touch_sensor.enableWheel();
  Serial.println("AT42QT2120 waiting for touch...");
  arrow.set(0, 50);
}

//--------------------------------------------------
void drawCircularTouches() {
  // sequence of the pads going from 1 o'clock, clockwise
  // when the connector is at the top
  // 7 10 11 9 8 6 5 3 2 0 1 4
  // int touchPads[12] = {7, 10, 11, 9, 8, 6, 5, 3, 2, 0, 1, 4};
  TKPoint center;
  center.set(width / 2, height / 2);
  float w = 90; // radius of our cirle pattern
  float r = 20; // radius of our circles on the pattern
  float angle = 0;
  float angleOffset = PI / 2 + TWO_PI / 24;

  for (uint8_t i = 0; i < 12; i++) {
    angle = (float)i * TWO_PI / 12;
    int x = center.x + w * cos(angleOffset + angle);
    int y = center.y + w * sin(angleOffset + angle);
    bool keyTouched = 0x01 & (keys >> i);
    if (keyTouched) {
      display.fillCircle(x, y, r, BLACK);
    } else {
      display.fillCircle(x, y, 5 * r / 6, GRAY);
    }
  }

  // display wheel angle
  TKPoint rotArrow(arrow);
  rotArrow.rotate(wheelAngle);
  rotArrow += center;
  display.drawFatLine(center.x, center.y, rotArrow.x, rotArrow.y, 2, BLACK);
}

//--------------------------------------------------
// setup the display
void setupDisplay() {
  SPI.begin(PIN_LCD_CLK, -1, PIN_LCD_DI, PIN_LCD_CS);
  display.begin();
  display.setRotation(2);
  display.clearDisplay();
}

//--------------------------------------------------
void drawCounter() {
  // draw something moving to show slow down in framerate
  unsigned long now = millis();
  float a = (float)now / 100.0f;
  int x = 20 + 10 * cos(a);
  int y = 20 + 10 * sin(a);
  display.fillCircle(x, y, 10, GRAY);
}

//--------------------------------------------------
void draw() {
  // Do drawing here
  // this function is called only 60 times/sec aka 60fps
  display.clearDisplayBuffer(); // clear our display buffer

  // draw something that moves
  drawCounter();

  // Draw touches
  drawCircularTouches();

  display.refresh(); // actually sends it to the display
}

//--------------------------------------------------
void setup() {
  Serial.begin((57600)); // start a serial port at 57600 BAUD
  delay(1000);           // wait 1000ms
  Serial.println();
  Serial.println(" Touches with AT42QT2120 "); // send a message over serial

  setupDisplay();

  // Turn our RGB led off
  pixels.begin();
  pixels.setPixelColor(0, 0, 0, 0);
  pixels.show();

  // Setup the display and show feedback
  display.setTextSize(3);
  display.setTextColor(BLACK);
  display.setCursor(5, 5); // where will we draw?
  display.println(" AT42QT2120 Capacitive touch demo");
  display.refresh(); // actually sends it to the display
  delay(500);

  // setup touch sensor and show feedback
  setupTouchSensor();
  display.clearDisplay();
  display.setCursor(5, 5);
  display.println(" Setup AT42QT2120");
  display.refresh();
  delay(100);

  display.clearDisplay();
}

//--------------------------------------------------
void loop() {

  unsigned long now = millis();      // get the current time
  if (now > lastFrame + 1000 / 60) { // do this every 60fps
    lastFrame = now;

    // pixels.setPixelColor(0, 255, 50, 0);
    // pixels.setBrightness(10.0f * (1 + sin((float)millis() / 500.0f)) / 2);
    // pixels.show();
    draw(); // draw to screen
  }

  if (change_line_activated) {
    change_line_activated = false;
    AT42QT2120::Status status = touch_sensor.getStatus();

    keys = status.keys;
    wheelAngle = (float)status.slider_or_wheel_position;
    // show that we can read the data
    Serial.print("status.keys: ");
    Serial.print(status.keys, BIN);
    Serial.print(" wheel: ");
    Serial.print(status.slider_or_wheel_position);
    Serial.println();
  }
}
