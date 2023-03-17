#include <CapacitiveSensor.h>

CapacitiveSensor cs_7_8 = CapacitiveSensor(7,8); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_9_10 = CapacitiveSensor(9,10); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;
char val;

void setup() {
    Serial.begin(115200);

    //establishContact();
    
}

void establishContact(){
  while (Serial.available() <= 0){
    Serial.println("A");
    delay(300);
  }
}
void loop() {
    //Serial.println("serial working")
  CSread();
  //countSensors();
}

void CSread() {
    long cs0 = cs_7_8.capacitiveSensor(80); //a: Sensor resolution is set to 80
    long cs1 = cs_9_10.capacitiveSensor(80); //a: Sensor resolution is set to 80

    if(cs0 > 0){
      Serial.print("Plant_A/");
      Serial.print(cs0);
      Serial.print("/");      
    }
    if(cs1 > 0){
      Serial.print("Plant_B/");
      Serial.println(cs1);     
    }
}
