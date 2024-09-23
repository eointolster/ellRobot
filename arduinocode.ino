// Pin definitions
const int motorPin1 = 7;
const int motorPin2 = 8;
const int motorPin3 = 9;
const int motorPin4 = 10;

void setup() {
  // Set all pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
}

void loop() {
  // Set pin 7 HIGH and the rest LOW
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(1000);  // Wait for 1 second

  // Set pin 8 HIGH and the rest LOW
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(1000);  // Wait for 1 second

  // Set pin 9 HIGH and the rest LOW
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(1000);  // Wait for 1 second

  // Set pin 10 HIGH and the rest LOW
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(1000);  // Wait for 1 second
}
