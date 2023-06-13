void setup() {
  Serial.begin(115200);
  Serial.println("Setting up");
}


void loop() {
  int value = fakeAnalogRead();
  Serial.println(value);
  delay(10);
}
