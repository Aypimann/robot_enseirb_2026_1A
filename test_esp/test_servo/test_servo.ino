// based on https://www.upesy.fr/blogs/tutorials/how-to-use-pwm-on-esp32-with-examples

#define SERVO_DATA 4

#define SERVO_PWM_FREQ 50
#define SERVO_PWM_RES 8

#define SERVO_NETRAL_POS 13
#define SERVO_LEFT_POS 1
#define SERVO_RIGHT_POS 26


#define DELAY 2000

void setup(){
  pinMode(SERVO_DATA, OUTPUT);

  ledcAttach(SERVO_DATA, SERVO_PWM_FREQ, SERVO_PWM_RES);

  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("ESP32 Echo Ready!");
}

void loop() {
  for(int i = 0; 1; i++){
    ledcWrite(SERVO_DATA, i);
    Serial.println(i);
    delay(DELAY);
  }
}
