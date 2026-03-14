#include "ESP32Encoder.h" // https://github.com/madhephaestus/ESP32Encoder.git 
 
#define CLK 15 // CLK ENCODER 
#define DT  2  // DT ENCODER 
#define PLUS 4
 
ESP32Encoder encoder;
 
void setup () { 
  pinMode(PLUS, OUTPUT);
  digitalWrite(PLUS, HIGH);
  encoder.attachHalfQuad ( DT, CLK );
  encoder.setCount ( 0 );
  Serial.begin ( 115200 );
}
 
void loop () {    
  long newPosition = encoder.getCount() / 2;
  Serial.println(newPosition);
} 
