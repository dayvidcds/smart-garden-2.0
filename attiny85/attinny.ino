#define BOMBA 0
#define DATA_INPUT 1

void setup() {                
  pinMode(BOMBA, OUTPUT);
  pinMode(DATA_INPUT, INPUT);
}

void loop() {
  if (digitalRead(DATA_INPUT) == HIGH) {
    digitalWrite(BOMBA, HIGH);
    delay(4000);
  }
  digitalWrite(BOMBA, LOW);
  delay(100);
}
