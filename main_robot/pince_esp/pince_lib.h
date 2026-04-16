#ifndef _PINCE
#define _PINCE

#include <Arduino.h>
#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
 
#define ENC_CLK_PINCE 16
#define ENC_DT_PINCE  4
#define MOT_A_PINCE   15
#define MOT_B_PINCE   2

#define PWM_FREQ_PINCE 100
#define PWM_RES_PINCE 8

extern const float default_pince_pos;
 
extern float goal_pince;
extern int is_pince_blocked;

float get_pince_pos();
void setup_pince();
int loop_pince();

#endif // !_PINCE
