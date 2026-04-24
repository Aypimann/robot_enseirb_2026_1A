void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("ESP32 Ready!");
  delay(1000);
}

void loop() {
  Serial.println("tkt");
  delay(500);
}
