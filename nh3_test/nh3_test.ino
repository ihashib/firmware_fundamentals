#include <ModbusMaster.h>
#include <HardwareSerial.h>
int slave;
ModbusMaster node;
HardwareSerial mserial(1);

void setup() {
  Serial.begin(115200);
  mserial.begin(9600);
  address_search(slave);
  node.begin(slave,mserial);
}


void loop() {
  uint8_t result = node.readHoldingRegisters(0, 6);
  Serial.println(result,HEX);
  if (result == node.ku8MBSuccess){
    Serial.print("NH3: ");
    Serial.println(node.getResponseBuffer(6));
   
  }
  delay(1000);
}
void address_search(int& address_slave ){
  for(int i = 15; i<24; i++){
    Serial.print("i: ");Serial.println(i);
    node.begin(i, mserial);
    uint8_t result = node.readHoldingRegisters(0, 2);
    if(result == node.ku8MBSuccess){
      address_slave = i;
      
      break;
    }
    else{
      Serial.print("No Slave found in: ");Serial.println(i);
    }
    delay(500);
  }
}
