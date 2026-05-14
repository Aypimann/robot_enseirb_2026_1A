#include "pinout.h"
#include "movement.h"

char team;
char next_goal;
unsigned itteration_being_arrived;
char ending = 0;

//pos initiale : 3/4 avant centré droite gauche

inline float uptime(){
  return 100;
}

void eating(){
  while(1); // TODO
}

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("C'est le bon code ?");
  setup_pins();
  team = get_team();
  next_goal = 'a';
  itteration_being_arrived = 0;
  rotate(team * -90);
}

void loop() {
  Serial.println("looping");
  //if (uptime() > 92) ending = 2;
  if (ending) {
    Serial.println("end");
    char debug[50];
    sprintf(debug, "mode : %d\n", ending);
    Serial.println(debug);
    Serial.println('0' + ending);
    if(ending == 1) eating();
    return;
  }
  Serial.println("looping2");
  if(-10 < dist_goal && dist_goal < 10) itteration_being_arrived ++;
  Serial.println("looping3");
  if (itteration_being_arrived > 50) {
    next_goal ++;
    itteration_being_arrived = 0;
    switch (next_goal){
      case 'b': //align with cases wrong orientation
        backforward(45); 
        break;
      case 'c': //align with cases right orientation
        rotate(team * 90);
        break;
      case 'd': //pushes cases
        backforward(265);
        break;
      case 'e': // steps back from cases
        backforward(-190);
        break;
      case 'f': // rotates to continue the way to eating zone
        rotate(team *-90);
        break;
      case 'g': // walks vertically to zone, pushes useless cases
        backforward(500);
        break;
      case 'h': // steps away from nuts to rotate
        backforward(-140);
        break;
      case 'i': // rotate to the eating zone
        rotate(team * 90);
        break;
      case 'j':
        backforward(195);
        break;
      default:
        ending = 1;
        break;
    }
  }
  Serial.println("looping3");
  movement_hand();
}
