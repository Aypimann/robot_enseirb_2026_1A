#ifndef _PINOUT_H
#define _PINOUT_H

#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
#include "Arduino.h"

// connect back detector on the same side as esp32's USB
extern Detector back_detector;
extern Detector front_detector;
 
// connect the left encoder near the esp32
extern ESP32Encoder encoder_l;
extern ESP32Encoder encoder_r;

//connect the left motor near current input
void set_l_motor(float power);
void set_r_motor(float power);

//blue : 1 pin high load; yellow : -1 pin to gnd
char get_team();

//connect the grab servo alone
void set_crab_servo(int pos);

void set_grab_servo(int pos);

void setup_pins();

#endif // !_PINOUT_H
