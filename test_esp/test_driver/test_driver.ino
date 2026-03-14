#define MR1pin1 15
#define MR1pin2 4

#define MR2pin1 4
#define MR2pin2 5

void setup() {
  // put your setup code here, to run once:
  pinMode(MR1pin1, OUTPUT);
  pinMode(MR1pin2, OUTPUT);
  pinMode(MR2pin1,  OUTPUT);
  pinMode(MR2pin2, OUTPUT);

  //(Optional)
  // pinMode(9,  OUTPUT); 
  // pinMode(10, OUTPUT);
  //(Optional)
}

void loop() {
  // put your main code here, to run repeatedly:

  //Controlling speed (0  = off and 255 = max speed):     
  //(Optional)
  // analogWrite(9, 100); //ENA  pin
  // analogWrite(10, 200); //ENB pin
  //(Optional)
  
  digitalWrite(MR1pin1,  HIGH);
  digitalWrite(MR1pin2, LOW);

  digitalWrite(MR2pin1, HIGH);
  digitalWrite(MR2pin2, LOW);
  delay(3000);

  digitalWrite(MR1pin1,  LOW);
  digitalWrite(MR1pin2, HIGH);

  digitalWrite(MR2pin1, LOW);
  digitalWrite(MR2pin2, HIGH);
  delay(3000);
}
