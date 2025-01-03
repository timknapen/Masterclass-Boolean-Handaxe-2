#include "housesProto.h"

static const int LEDS[NUMLEDS] = {1, 3, 5, 7, 9, 11, // bottom row ( RED )
                                  0, 2, 4, 6, 8, 10};

// LEDS - pca9685
bool bLEDsEnabled = false;
int toggles[NUMBUTTONS] = {0, 0, 0, 0, 0, 0};

int buttonPins[NUMBUTTONS] = {
    BTN_PIN_BOOT, // BOOT
    BTN_PIN_A,    // A
    BTN_PIN_B,    // B
    BTN_PIN_X,    // X
    BTN_PIN_Y,    // Y
    BTN_PIN_Z,    // Z  // only on 1M houses

};

unsigned long lastEvent = 0;
unsigned long lastLEDupdate = 0;
// when did a button event last fire? for long press scrolling

Adafruit_PWMServoDriver pwmDriver = Adafruit_PWMServoDriver(0x40, Wire);

//-------------------------------------------
void pca_enable() {
  bLEDsEnabled = true;
  digitalWrite(PIN_PCA_ENABLE, LOW);
}

//-------------------------------------------
void pca_disable() {
  bLEDsEnabled = false;
  digitalWrite(PIN_PCA_ENABLE, HIGH);
}

//-------------------------------------------
void pca_setup() {
  pinMode(PIN_PCA_ENABLE, OUTPUT);
  pca_disable();

  // Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setPins(PIN_SDA, PIN_SCL);
  pwmDriver.begin();
  Serial.println("Done setting up pca9685");

  for (int i = 0; i < 16; i++) {
    pwmDriver.setPWM(i, 0, 4095 - MAXBRIGHTNESS * 0);
  }
}

//-------------------------------------------
void pca_demo() {
  // switch off all LEDS
  for (int i = 0; i < 16; i++) {
    pwmDriver.setPin(i, 0, true);
  }

  pca_enable();

  // run through all LEDS
  // switch off all LEDS

  for (int i = 0; i < NUMLEDS; i++) {
    for (int x = 0; x < NUMLEDS; x++) {
      if (i == x) {
        pwmDriver.setPin(LEDS[x], 4095, true);
      } else {
        pwmDriver.setPin(LEDS[x], 0, true);
      }
    }
    delay(100);
  }
  // switch off all LEDS
  for (int i = 0; i < 16; i++) {
    pwmDriver.setPin(i, 0, true);
  }
}

//-------------------------------------------
void pca_update() {
  /*
// return;
int period = 500; // 1 sec
float pct = (float)(millis() % period) / (float)period;
// Serial.println((int)(100 * pct));
int selButton = -1;
for (int i = 0; i < NUMBUTTONS; i++) {
  if (buttons.pressed(i)) {
    selButton = i;
  }
}
if (selButton != -1) {
  for (int i = 0; i < NUMBUTTONS; i++) {
    if (i == selButton) {
      if (toggles[i] == 1) {
        toggles[i] = 2;
      } else {
        toggles[i] = 1;
      }
    } else {
      toggles[i] = 0;
    }
  }
}

float brightness = 0.5 + 0.5 * sinf(TWO_PI * pct);

// toggle OFF/RED/WHITE
int t = millis() % period; // time pos
if (gameState == GAME_PLAY) {
  int selPage = paramManager.getSelPage();
  int activeParam = paramManager.getActiveParam();
  for (int i = 0; i < NUMBUTTONS; i++) {
    if (i == activeParam && t < period / 2) {
      pwmDriver.setPin(LEDS[i + 6], 4095, true); // white all off
    } else {
      pwmDriver.setPin(LEDS[i + 6], 0, true); // white all off
    }
    if (i == selPage) {
      pwmDriver.setPin(LEDS[i], 4095, true); // red
    } else {
      pwmDriver.setPin(LEDS[i], 0, true); // red
    }
  }
} else {
  for (int i = 0; i < NUMBUTTONS; i++) {
    if (toggles[i] == 0) {
      pwmDriver.setPWM(LEDS[i + 6], 0, 4095);
      pwmDriver.setPWM(LEDS[i], 0, 4095);
    } else if (toggles[i] == 1) { // RED
      pwmDriver.setPWM(LEDS[i], 0, 4095 - MAXBRIGHTNESS);
      pwmDriver.setPWM(LEDS[i + 6], 0, 4095);
    } else { // WHITE
      pwmDriver.setPWM(LEDS[i + 6], 0, 4095 - MAXBRIGHTNESS);
      pwmDriver.setPWM(LEDS[i], 0, 4095);
    }
  }
}
//*/
}

#ifdef USE_POTS

int btnPins[NUMBUTTONS] = {
    BTN_PIN_BOOT, BTN_PIN_A, BTN_PIN_B, BTN_PIN_X, BTN_PIN_Y, BTN_PIN_Z,
};

//-------------------------------------------
void setupButtons() {

 Serial.println("Setup buttons");
  for (int i = 0; i < NUMBUTTONS; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(btnPins[i]);
  }
  Serial.println();
}



int potPins[NUMPOTS] = {
    PIN_POT_A, //
    PIN_POT_B, //
    PIN_POT_C, //
    PIN_POT_D, //
    PIN_POT_E, //
    PIN_POT_F, //
               // PIN_BAT_MEASURE //
};

float potVals[NUMPOTS];

//-------------------------------------------
void setupPots() {
  Serial.println("Setup pots");
  for (int i = 0; i < NUMPOTS; i++) {
    pinMode(potPins[i], INPUT);
    potVals[i] = 0.5;
    Serial.print(i);
    Serial.print(" ");
  }
  Serial.println();
  pinMode(PIN_BAT_MEASURE, INPUT);
}

//-------------------------------------------
void drawPots() {
  // implement
}

//-------------------------------------------
void updatePots() {
  for (int i = 0; i < NUMPOTS; i++) {
    int potVal = analogRead(potPins[i]);
    potVals[i] = (float)potVal / 4096.0f;
  }
}

//-------------------------------------------
float getPotVal(int i) {
  if (i >= 0 && i < NUMPOTS) {
    return potVals[i];
  }
  return 0;
}

#endif