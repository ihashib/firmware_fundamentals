/*
  on Sep 23, 2020
  by MohammedDamirchi
  https://electropeak.com/learn/
*/

#define m A2

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

}


// the loop routine runs over and over again forever:
void loop() {
  if (digitalRead(m) == HIGH) {
    Serial.println("5");

  } else {
    Serial.println("0");
  }
  delay(20);
  } 
