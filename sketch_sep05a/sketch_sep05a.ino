#define test A2
int motion_sensor_value;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  motion_sensor_value = digitalRead(test);
  Serial.print("Motion Sensor:"); Serial.println(motion_sensor_value);
}
