#ifndef _TOUCHSENSOR_H_
#define _TOUCHSENSOR_H_

#include "Adafruit_MPR121.h"
#include "pins_1M_houses.h"


// Capacitive touch sensor

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setupTouchSensor();
void updateTouchSensor();
void drawHorizontalTouches() ;
void drawCircularTouches();


#endif