#include "SPIFFS.h"

String sensor_val = "T:8.57,H:5.37,CO2:4048";
float temparature, humidity, Co2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
      Serial.println("Error mounting SPIFFS");
      return;
  }

  str_parse(sensor_val, temparature, humidity, Co2);

  //SPIFFS.format();

  write_file("/sensors.txt", temparature, humidity, Co2);

  read_file("/sensors.txt");

  //append_file("/sensor.txt", temparature, humidity, Co2);

  //delete_file("/sensor.txt");
}

void loop() {
  // put your main code here, to run repeatedly:

}

void str_parse(String str_sensor_val, float& temp, float& hum, float& co2){
    char* pointer;
    char char_sensor_val[str_sensor_val.length()+1];

    memset(char_sensor_val, '\0', sizeof(str_sensor_val));
    strcpy(char_sensor_val, str_sensor_val.c_str());

    temp = atof(strtok(strtok_r(char_sensor_val, ",", &pointer), "T:"));
    hum = atof(strtok(strtok_r(pointer, ",", &pointer), "H:"));
    strtok_r(pointer, ":", &pointer);
    co2 = atof(pointer);
}

void write_file(char* path, float temp, float hum, float Co2){
   Serial.printf("Writing file: %s\r\n", path);

   File file = SPIFFS.open(path, FILE_WRITE);
   if(!file){
      Serial.println("file not found");
      return;
   }

   file.print(temp);
   file.print(" ");
   file.print(hum);
   file.print(" ");
   file.print(Co2);

   file.close();
}

void read_file(char* path){
   Serial.printf("Reading file: %s\r\n", path);

   File file = SPIFFS.open(path, FILE_READ);
   if(!file || file.isDirectory()){
       Serial.println("failed to open file for reading");
       return;
   }

   Serial.println("read from file:");
   while(file.available()){
      Serial.write(file.read());
   }

   file.close();
}

//does not add to the write file function, adds to append itself. 
void append_file(char* path, float& temp, float& hum, float& co2){
  Serial.printf("Writing file: %s\r\n", path);

   File file = SPIFFS.open(path, FILE_APPEND);
   if(!file){
      Serial.println("file not found");
      return;
   }

   file.print("hello world");

   file.close();
}

void delete_file(char* path){
  Serial.print("Deleting file:"); Serial.println(path);
  if(SPIFFS.remove(path)){
    Serial.println("File removed");
  }
  else{
    Serial.println("File remove failed");
  }
}

void format(){
  Serial.println("Before formating");
  list_all_files();
  SPIFFS.format();
  Serial.println("After formating");
  list_all_files();
}

void list_all_files(){
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  uint8_t count=0;
  while(file){
    Serial.print("FILE: ");
    Serial.println(file.name());
    file = root.openNextFile();
    count++;
  }
  Serial.print("total files : ");
  Serial.println(count);
}

//prints total file count
void fileCount(){
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  int count=0;
  while(file){
    count++;
    file = root.openNextFile();
  }
  Serial.print("total files : ");
  Serial.println(count);
}
