void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  Serial.println("ESP32 Echo Ready!");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
  }
}
