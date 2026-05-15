#ifndef _PINOUT_H
#define _PINOUT_H

#include "detector.h"
#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
#include "Arduino.h"

extern Detector detector;
 
extern ESP32Encoder encoder_l;
extern ESP32Encoder encoder_r;

void set_l_motor(float power);
void set_r_motor(float power);

void set_l_servo(int pos);
void set_r_servo(int pos);

void setup_pins();

#endif // !_PINOUT_H
