int analogPin = 13;
int val = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up");
}

void loop() {
  val = analogRead(analogPin);
  Serial.println(val);
}