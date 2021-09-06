#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <stdio.h> 
#include <string.h>
// don't use 9, 10 pin
#define mode_select_button A0
#define start_stop_reset A1
#define relay 11
#define buzzer 8
#define interruptPin  2
#define idle_time 2

//adding new for motion sensor
#define motion_sensor A2
int motion_sensor_value;
int motion_time_out;
bool motion = false;
bool status_motion_msg = false;

byte rotatetext = 0;
bool alarm = false;
bool door = false;
bool buzzer_flag = false;
bool idle_flag = false;

int menu_stat,stop_rst;
int lastButtonState_1 = 1;
int lastButtonState_2 = 1;
int debounce_delay = 50;
int mode, stp;
int mode_1_2_3 = 0;

int max_mode = 2;

int status_door;
int j,k;
int lo;

int status_door_msg = 0;  //used as door open/close flag

int mode_1_time = 2;//2
int mode_2_time = 4;//4
int mode_3_time = 6;//6

int alarm_sec;
double fshow_min;
int show_min,show_sec;
double fshow_min1;
int show_min1,show_sec1;
int sec;
char f_sec[10];
float f_show_sec1;

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

void setup() {
  Serial.begin(9600);
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  
  display.setRotation(rotatetext);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.print("WELCOME");
  
  display.setTextSize(1);
  display.setCursor(18,26);
  display.print("AQUALINK");
  display.setCursor(0,40);
  display.print("BANGLADESH LTD");
  display.display();
  delay(3000);
  display.clearDisplay();
  
  cli();
  
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(relay,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(relay,LOW);
  digitalWrite(buzzer,LOW);
  
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15420;// = (16*10^6) / (1*1024) - 1 (must be <65536)//15624//15420
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();

}

ISR(TIMER1_COMPA_vect){
  //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if(alarm == true && door == false && motion == false){
    alarm_sec--;
    Serial.println(alarm_sec);
    digitalWrite(relay,HIGH);
    if(alarm_sec <= 0){
      alarm = false;
      sec = 0;
      alarm_sec = 0;
      digitalWrite(relay,LOW);
      buzzer_flag = true;
      //warning_tone(); 
    }
  }
  //motion time out of 10 sec if motion is detected. 
  if(motion == true){
    Serial.print("motion_time_out: "); Serial.println(motion_time_out);
    motion_time_out++;
    digitalWrite(relay, LOW);
    if(digitalRead(motion_sensor) == 1){
      motion_time_out = 9;
    }
    if(motion_time_out > 10 and digitalRead(motion_sensor) == 0){
      motion_time_out = 0;
      motion = false; 
      status_motion_msg = false;
    }
  }
}

void loop(){
  while(alarm == false){
    if(idle_flag == true && status_door_msg == 0){
      display_wait();
      button_loop();
      door_loop();
    }
    //door_loop();
    else if(idle_flag == false && status_door_msg == 1){
      display_wait();
      button_loop();
      door_loop();
      if(buzzer_flag == true){
        warning_tone();
        buzzer_flag = false;
      } 
    }
    else if(idle_flag == false && status_door_msg == 0) {
      if( j > idle_time*100){
        idle_flag = true;
      }
      door_loop();
      //Serial.println(stp);
      set_alarm_loop();
      delay(10); 
      j++;
      //Serial.println(j);
    }
  }
  
  while(alarm == true){
    status_door_msg = 1;
    door_loop();
    //Serial.println(stp);
    reset_timer();
    show_time_alarm();
    if(mode_1_2_3 == 0){
      display_M1("Mode 1:");    
    }
    if(mode_1_2_3 == 1){
      display_M1("Mode 2:");
    }
    if(mode_1_2_3 == 2){
      display_M1("Mode 3:");
    }
    //delay(50);
  } 
}

void reset_timer(){
    button_loop();
    if(stp==1){
      stp = 0;
      alarm_sec = 0;
      sec = 0;
      alarm = false;
      status_door_msg = 0;
      select_tone();
      digitalWrite(relay,LOW);
    }
}

void set_alarm_loop(){

  if(idle_flag == true){
    display_wait();
  }
  else {
    button_loop();
    if(mode==1 && mode_1_2_3 <= max_mode){
      mode = 0;
      mode_1_2_3++;
      if(mode_1_2_3>max_mode){
         mode_1_2_3 = 0;
      }
       select_tone();
    }
    if(mode_1_2_3 == 0){
      sec = 60*mode_1_time;
      display_M("Mode 1:");
    }
    if(mode_1_2_3 == 1){
      sec = 60*mode_2_time;
      display_M("Mode 2:");     
    }
    if(mode_1_2_3 == 2){
      sec = 60*mode_3_time;
      display_M("Mode 3:");   
    }  
  
    if(stp==1 && sec>0 && door == false){
      stp = 0;
      alarm = true;
      alarm_sec = sec;
      digitalWrite(relay,HIGH);
      select_tone();
    }
  } 
}

void button_loop(){
  menu_stat = digitalRead(mode_select_button);
  stop_rst = digitalRead(start_stop_reset);
  if (menu_stat != lastButtonState_1) {
    if (menu_stat == 0){
      mode = 1;
      j = 0;
      lo = 0;
      status_door_msg = 0;
      idle_flag = false;
    }
    else mode = 0;
    delay(debounce_delay);
    // select_tone(); 
  }
  
  lastButtonState_1 = menu_stat;

  if (stop_rst != lastButtonState_2){
    if (stop_rst == 0){
      j = 0;
      lo = 0;
      status_door_msg = 0;
      idle_flag = false;
      stp = 1;
    }
    else stp = 0;
    delay(debounce_delay);
   
  }
 
  lastButtonState_2 = stop_rst;
}

void select_tone(){
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);
}

void warning_tone(){
  digitalWrite(buzzer,HIGH);
  delay(400);
  digitalWrite(buzzer,LOW);
  delay(300);
  digitalWrite(buzzer,HIGH);
  delay(400);
  digitalWrite(buzzer,LOW);
  delay(300);
  digitalWrite(buzzer,HIGH);
  delay(400);
  digitalWrite(buzzer,LOW);
}


void display_wait(){
  if( lo < 200 && status_door_msg == 0 && status_motion_msg == false){
    display.clearDisplay();
    display.setRotation(rotatetext);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print("Please!");
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(8,30);
    display.print("Select Mode");
    display.display();
  }
  else if( 200 < lo < 280 && status_door_msg == 0 && status_motion_msg == false){
    display.clearDisplay();
    display.setRotation(rotatetext);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(20,0);
    display.print("Stay");
    display.setCursor(0,15);
    display.print("Safe...");
    
    display.setTextSize(1);
    display.setCursor(18,32);
    display.print("AQUALINK");
    display.setCursor(0,40);
    display.print("BANGLADESH LTD");
    display.display();
  }
  if(status_door_msg == 1 && status_motion_msg == false){
    display.clearDisplay();
    display.setRotation(rotatetext);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print("Done!!!");
    display.setTextSize(1);
    display.setCursor(20,32);
    display.print("Please");
    display.setCursor(0,40);
    display.print("Open The Door");
    display.display();
  }
  if(status_motion_msg){
    display.clearDisplay();
    display.setRotation(rotatetext);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(8,10);
    display.print("Motion");
    display.setTextSize(1);
    display.setCursor(16,32);
    display.print("detected!!");
    display.display();
  }
  
  if(lo > 280 && status_door_msg == 0){
    lo = 0;
  }
  //Serial.print("lo value: ");Serial.println(lo);
  lo++; 
}


void display_M(char*mode_name){
  if(status_motion_msg == false){
    show_time_No_alarm();
    display.clearDisplay();
    display.setRotation(rotatetext);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print(mode_name);
    display.setTextSize(3);
    display.setTextColor(BLACK);
    display.setCursor(0,20);
    if(show_min<10){
      display.print("0");display.print(show_min);
    }
    else{
      display.print(show_min);
    }
    display.setCursor(33,20);
    display.print(":");
    display.setCursor(47,20);
    if(show_sec<10){
      display.print("0");display.print(show_sec);
    }
    else{display.print(show_sec);}
    
    display.display();
  }
}

void display_M1(char*mode_name){
  if(status_motion_msg == false){
    show_time_No_alarm();
    display.clearDisplay();
    display.setRotation(rotatetext);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print(mode_name);
    display.setTextSize(3);
    display.setTextColor(BLACK);
    display.setCursor(0,20);
    if(show_min1<10){
      display.print("0");display.print(show_min1);
    }
    else{
      display.print(show_min1);
    }
    display.setCursor(33,20);
    display.print(":");
    display.setCursor(47,20);
  
    if(f_show_sec1 <= 9){
      display.print("0");display.print(f_show_sec1,0);
    }
    else{display.print(f_show_sec1,0);}
    
    display.display();
  }
}


void door_loop(){
  status_door = digitalRead(interruptPin);
  motion_sensor_value = digitalRead(motion_sensor);
  //Serial.println(status_door);
  if(status_door == 0 and motion_sensor_value == 0 and status_motion_msg == false){  
    door = false;
    motion = false;
  }
  else {
    motion = true;
    status_motion_msg = true;
    display_wait();
    door = true; 
    digitalWrite(relay,LOW);
    status_door_msg = 0; 
  }
}

void show_time_No_alarm(){
    show_min = sec/60;
    fshow_min = ((double)sec/60.00);
    show_sec = ((fshow_min-show_min)*60.00);
}

void show_time_alarm(){
    show_min1 = alarm_sec/60;
    fshow_min1 = ((double)alarm_sec/60);
    show_sec1 = ((fshow_min1-show_min1)*60);
    f_show_sec1 = ((fshow_min1-show_min1)*60);
}
