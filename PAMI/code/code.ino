#include "pinout.h"
#include "movement.h"

unsigned itteration_being_arrived;
char ending = 0;

//pos initiale : 3/4 avant centré droite gauche

inline float uptime(){
  return micros()/1000000.;
}

void eating(){
  while(1); // TODO
}

void setup() {
  Serial.begin(115200);
  delay(2000);//85500);
  while (!Serial) ;
  Serial.println("Code de pami");
  setup_pins();
  Serial.println("Fin du setup");
  itteration_being_arrived = 0;
  backforward(450);
}

void loop() {
  Serial.println("cp1");
  if (itteration_being_arrived > 50) ending = 1;
  if (uptime() > 92) ending = 2;
  Serial.println("cp2");
  if (ending) {
    set_l_motor(0);
    set_r_motor(0);
    Serial.println("end");
    char debug[50];
    sprintf(debug, "end : %d, time: %f\n", ending, uptime());
    Serial.println(debug);
    if(ending == 1) eating();
    return;
  }
  Serial.println("cp3");
  if(-1 < dist_goal && dist_goal < 1) itteration_being_arrived ++;
  Serial.println("cp4");
  movement_hand();
}
