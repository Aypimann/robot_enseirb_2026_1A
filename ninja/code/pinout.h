#ifndef _PINOUT_H
#define _PINOUT_H

#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
#include "Arduino.h"
 
// connect the left encoder near the esp32
ESP32Encoder encoder_l;
ESP32Encoder encoder_r;

//connect the left motor near current input
void set_l_motor(float power);
void set_r_motor(float power);

char get_team();

//connect the grab servo alone
void set_crab_servo(int pos);
void set_grab_servo(int pos);

void setup();

#endif // !_PINOUT_H
