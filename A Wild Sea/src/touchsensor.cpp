#include "touchSensor.h"

Adafruit_MPR121 cap = Adafruit_MPR121();

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
  // draw touches like a horizontal "piano"
  for (uint8_t i = 0; i < 12; i++) {
    if (currtouched & _BV(i)) {
      // we are touching pin i
      int w = width / 12;
      int x = i * w;
      int y = height - 30;
    //   display.fillRect(x, y, w, w, BLACK);
    }
  }
}

//--------------------------------------------------
void drawCircularTouches() {
  // draw touches in a circle
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
    //   display.fillCircle(x, y, r, BLACK);
    } else {
    //   display.fillCircle(x, y, r, GRAY);
    }
  }
}
