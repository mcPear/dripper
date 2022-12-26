#define PUMP_PIN 8

//setup
void setup() {
  pinMode(PUMP_PIN, OUTPUT);
}

void boom(int delay_){
  digitalWrite(PUMP_PIN, HIGH);
  delay(30);
  digitalWrite(PUMP_PIN, LOW);
  delay(delay_);
}

void loop() {
  boom(500);
  boom(500);
  boom(250);
  boom(250);
  boom(500);
  boom(250);
  boom(250);
  boom(250);
  boom(500);
  boom(250);
  boom(2000);
}
