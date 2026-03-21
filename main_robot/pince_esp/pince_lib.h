#ifndef _PINCE
#define _PINCE

#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
 
#define ENC_CLK_PINCE 16
#define ENC_DT_PINCE  4
#define MOT_A_PINCE   15
#define MOT_B_PINCE   2

#define PWM_FREQ_PINCE 100
#define PWM_RES_PINCE 8

#define DEFAULT_PINCE_POS 160.0
 
float goal_pince = DEFAULT_PINCE_POS;
int is_pince_blocked = 0;

float get_pince_pos();
void setup_pince();
void loop_pince();

#endif // !_PINCE
