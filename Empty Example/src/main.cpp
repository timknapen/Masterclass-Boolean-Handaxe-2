#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define RGB_LED_PIN 38
Adafruit_NeoPixel strip(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

#define NUM_COLORS 6
int colors[NUM_COLORS * 3] = {1, 0, 0, 1, 1, 0, 0, 1, 0,
                              0, 1, 1, 0, 0, 1, 1, 0, 1};
int colorCounter = 0;

// put function declarations here:

void setup() {
  Serial.begin(57600);

  // setup RGB LED
  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();  // Turn OFF all pixels ASAP
  strip.setBrightness(20);
}

void loop() {
  if (colorCounter == 0) {
    Serial.print("Boolean Handaxe ");
    Serial.println(millis() / 1000);
  }

  int i = colorCounter * 3;
  int r = 255 * colors[i];
  int g = 255 * colors[i + 1];
  int b = 255 * colors[i + 2];

  strip.setPixelColor(0, strip.Color(r, g, b));
  strip.show();

  colorCounter++;
  colorCounter %= NUM_COLORS;

  delay(500);
}
