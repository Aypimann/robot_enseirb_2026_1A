#define DRAIN 15
#define GRILLE 2
#define SOURCE 4

void setup() {
  pinMode(DRAIN, OUTPUT);
  pinMode(GRILLE, OUTPUT);
  pinMode(SOURCE, INPUT);

  delay(5000);

  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("ESP32 Echo Ready!");

  digitalWrite(DRAIN, HIGH);
  digitalWrite(DRAIN, HIGH);
  delay(1000);
  Serial.println(analogRead(SOURCE));

  digitalWrite(DRAIN, HIGH);
  digitalWrite(DRAIN, HIGH);
  delay(1000);
  Serial.println(analogRead(SOURCE));
}

void loop() {
  Serial.println(analogRead(SOURCE));
}
