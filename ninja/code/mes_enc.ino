#include "pinout.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("ESP32 Ready!");
  setup_pins();
  // set_r_motor(1);
  // set_l_motor(1);

}

void loop() {
  delay(100);
  set_r_motor(0.5);
  Serial.println(encoder_l.getCount());
  /*
  int lc = encoder_l.getCount();
  int rc = encoder_r.getCount();
  if(lc + rc > 10000){
    set_r_motor(0);
    set_l_motor(0);
    Serial.print("left :");
    Serial.print(lc);
    Serial.print("left :");
    Serial.println(rc);
  }

  if(lc - rc < -20) set_r_motor(0.5);
  else              set_r_motor(1.0);

  if(lc - rc >  20) set_l_motor(0.5);
  else              set_l_motor(1.0);
  */
}
