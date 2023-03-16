#include <CapacitiveSensor.h>

CapacitiveSensor cs_7_8 = CapacitiveSensor(7,8); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
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
}

void CSread() {
    long cs = cs_7_8.capacitiveSensor(80); //a: Sensor resolution is set to 80

    Serial.println("Plant_A");
    Serial.println(cs);    
}
