//#define GND 34
#define DE 13
#define S1 12
#define S0 14
#define S3 17
#define S2 
#define OUT 2
//#define VCC 5

unsigned long duration;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(DE, OUTPUT);

  pinMode(OUT, INPUT);

  // pinMode(GND, OUTPUT);
  // pinMode(VCC, OUTPUT);


  digitalWrite(S0,  HIGH);
  digitalWrite(S1,  HIGH);
  digitalWrite(DE,  LOW);

  // digitalWrite(GND, LOW);
  // digitalWrite(VCC, HIGH);

  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("ESP32 Ready!");
}

void loop() {
  digitalWrite(S2,  HIGH);
  digitalWrite(S3,  HIGH);

  duration = pulseInLong(OUT, LOW, 200000);
  Serial.println("duration");
}
