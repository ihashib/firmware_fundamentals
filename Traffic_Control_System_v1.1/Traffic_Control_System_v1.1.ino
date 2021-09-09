#define ledA1 2
#define ledA2 3
#define ledA3 4

#define ledB1 5
#define ledB2 6
#define ledB3 7

#define ledC1 8
#define ledC2 9
#define ledC3 10

#define ledD1 12
#define ledD2 11
#define ledD3 13

int a1, a2, b1, b2, c1, c2, d1, d2;
char next_lane = 'b';

void setup() {

  Serial.begin(9600);

  pinMode(ledA1, OUTPUT);
  pinMode(ledA2, OUTPUT);
  pinMode(ledA3, OUTPUT);

  pinMode(ledB1, OUTPUT);
  pinMode(ledB2, OUTPUT);
  pinMode(ledB3, OUTPUT);

  pinMode(ledC1, OUTPUT);
  pinMode(ledC2, OUTPUT);
  pinMode(ledC3, OUTPUT);

  pinMode(ledD1, OUTPUT);
  pinMode(ledD2, OUTPUT);
  pinMode(ledD3, OUTPUT);



}

void loop() {

  readSensor();
  //normal operation
  if (!a1 and !a2 and !b1 and !b2 and !c1 and !c2 and !d1 and !d2 and next_lane == 'b'){
    roadBopen(false);
    next_lane = 'd';
  }
  else if (!a1 and !a2 and !b1 and !b2 and !c1 and !c2 and !d1 and !d2 and next_lane == 'd'){
    roadDopen(false);
    next_lane = 'a';
  }
  else if (!a1 and !a2 and !b1 and !b2 and !c1 and !c2 and !d1 and !d2 and next_lane == 'a'){
    roadAopen(false);
    next_lane = 'c';
  }
  else if (!a1 and !a2 and !b1 and !b2 and !c1 and !c2 and !d1 and !d2 and next_lane == 'c'){
    roadCopen(false);
    next_lane = 'b';
  }
  
  //when one lane is occupied
  else if((a1 or a2) and !b1 and !b2 and !c1 and !c2 and !d1 and !d2){
    roadAopen(true);
    next_lane = 'c';
  }
  else if(!a1 and !a2 and (b1 or b2) and !c1 and !c2 and !d1 and !d2){
    roadBopen(true);
    next_lane = 'd';
  }
  else if(!a1 and !a2 and !b1 and !b2 and (c1 or c2) and !d1 and !d2){
    roadCopen(true); 
    next_lane = 'b';
  }
  else if(!a1 and !a2 and !b1 and !b2 and !c1 and !c2 and (d1 or d2)){
    roadDopen(true); 
    next_lane = 'a';
  }

  //when 2 lanes detect vehicle
  else if((a1 or a2) and (b1 or b2) and !c1 and !c2 and !d1 and !d2){
    roadAopen(false);
    //ab
    if((a1 or a2) and (b1 or b2) and !c1 and !c2 and !d1 and !d2){
      roadBopen(false); 
    }
    next_lane = 'd';
  }
  else if(!a1 and !a2 and (b1 or b2) and (c1 or c2) and !d1 and !d2){
    roadBopen(false);
    //bc 
    if(!a1 and !a2 and (b1 or b2) and (c1 or c2) and !d1 and !d2){
      roadCopen(false); 
    }
    next_lane = 'b';
  }
  //ca
  else if((a1 or a2) and !b1 and !b2 and (c1 or c2) and !d1 and !d2){
    roadCopen(false); 
    if((a1 or a2) and !b1 and !b2 and (c1 or c2) and !d1 and !d2){
      roadAopen(false);
    }
    next_lane = 'c';
  }
  //bd
  else if(!a1 and !a2 and (b1 or b2) and !c1 and !c2 and (d1 or d2)){
    roadDopen(false); 
    if(!a1 and !a2 and (b1 or b2) and !c1 and !c2 and (d1 or d2)){
      roadBopen(false);
    }
    next_lane = 'd';
  }
  //ad
  else if((a1 or a2) and !b1 and !b2 and !c1 and !c2 and (d1 or d2)){
    roadDopen(false); 
    if((a1 or a2) and !b1 and !b2 and !c1 and !c2 and (d1 or d2)){
      roadAopen(false);
    }
    next_lane = 'c';
  }
  //dc
  else if(!a1 and !a2 and !b1 and !b2 and (c1 or c2) and (d1 or d2)){
    roadDopen(false); 
    if(!a1 and !a2 and !b1 and !b2 and (c1 or c2) and (d1 or d2)){
      roadCopen(false);
    }
    next_lane = 'b';
  }
}

void readSensor()
{
  a1 = analogRead(A7);
  a2 = analogRead(A6);
  b1 = analogRead(A4);
  b2 = analogRead(A5);
  c1 = analogRead(A1);
  c2 = analogRead(A0);
  d1 = analogRead(A3);
  d2 = analogRead(A2);

  if (a1 < 400) a1 = 1; else a1 = 0; if (a2 < 400) a2 = 1; else a2 = 0;
  if (b1 < 400) b1 = 1; else b1 = 0; if (b2 < 400) b2 = 1; else b2 = 0;
  if (c1 < 400) c1 = 1; else c1 = 0; if (c2 < 400) c2 = 1; else c2 = 0;
  if (d1 < 400) d1 = 1; else d1 = 0; if (d2 < 400) d2 = 1; else d2 = 0;
  
  Serial.print(a1);
  Serial.print("\t");
  Serial.print(a2);
  Serial.print("\t");
  Serial.print(b1);
  Serial.print("\t");
  Serial.print(b2);
  Serial.print("\t");
  Serial.print(c1);
  Serial.print("\t");
  Serial.print(c2);
  Serial.print("\t");
  Serial.print(d1);
  Serial.print("\t");
  Serial.print(d2);
  Serial.println("\t");

}

void roadAopen(bool shift)
{
  digitalWrite(ledA3, LOW);

  digitalWrite(ledA1, HIGH);
  digitalWrite(ledB3, HIGH);
  digitalWrite(ledC3, HIGH);
  digitalWrite(ledD3, HIGH);
  delay(10000);
  digitalWrite(ledA1, LOW);
  if(!shift){
    digitalWrite(ledA2, HIGH);
    delay(1000);
    digitalWrite(ledA2, LOW);
  }
  readSensor();
}

void roadBopen(bool shift)
{
  digitalWrite(ledB3, LOW);

  digitalWrite(ledA3, HIGH);
  digitalWrite(ledB1, HIGH);
  digitalWrite(ledC3, HIGH);
  digitalWrite(ledD3, HIGH);
  delay(10000);
  digitalWrite(ledB1, LOW);
  if(!shift){
    digitalWrite(ledB2, HIGH);
    delay(1000);
    digitalWrite(ledB2, LOW);
  }
  readSensor();
}

void roadCopen(bool shift)
{
  digitalWrite(ledC3, LOW);

  digitalWrite(ledA3, HIGH);
  digitalWrite(ledB3, HIGH);
  digitalWrite(ledC1, HIGH);
  digitalWrite(ledD3, HIGH);
  delay(10000);
  digitalWrite(ledC1, LOW);
  if(!shift){
    digitalWrite(ledC2, HIGH);
    delay(1000);
    digitalWrite(ledC2, LOW);
  }
  readSensor();
}

void roadDopen(bool shift)
{
  digitalWrite(ledD3, LOW);

  digitalWrite(ledA3, HIGH);
  digitalWrite(ledB3, HIGH);
  digitalWrite(ledC3, HIGH);
  digitalWrite(ledD1, HIGH);
  delay(10000);
  digitalWrite(ledD1, LOW);
  if(!shift){
    digitalWrite(ledD2, HIGH);
    delay(1000);
    digitalWrite(ledD2, LOW);
  }
  readSensor();
}
