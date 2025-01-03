#ifndef _HOUSES_PROTO_H_
#define _HOUSES_PROTO_H_

#include "pins_1M_houses.h"

// specific 1m house proto hardware functionality

// PCA9685 PWM driver
#include <Adafruit_PWMServoDriver.h>
#define MAXBRIGHTNESS 4096
#define NUMLEDS 12

void pca_enable();
void pca_disable();
void pca_setup();
void pca_demo();
void pca_update();

// Define button names
// labeled A - F from left to right on the 1M houses PCB
#define BTN_A 0 // DOWN // Boot
#define BTN_B 1 // LEFT // A
#define BTN_C 2 // UP   // B
#define BTN_D 3 // RIGHT // X
#define BTN_E 4 // A // Y
#define BTN_F 5 // B // Z

// special buttons
#define BTN_MENU BTN_A
#define BTN_BACK BTN_B
#define BTN_OK BTN_C
#define BTN_UP BTN_D
#define BTN_DOWN BTN_E

void drawButtons();

#define USE_POTS

#ifdef USE_POTS

extern int btnPins[NUMBUTTONS];

void setupButtons();
void setupPots();
void drawPots();
void updatePots(); // also updates paramManager

float getPotVal(int i);
#endif

#endif