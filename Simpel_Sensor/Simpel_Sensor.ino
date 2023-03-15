#include <CapacitiveSensor.h>

CapacitiveSensor cs_7_8 = CapacitiveSensor(7,8); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;

void setup() {
    Serial.begin(9600);
  
}

void loop() {
    //Serial.println("serial working");
    CSread();
}

void CSread() {
    long cs = cs_7_8.capacitiveSensor(80); //a: Sensor resolution is set to 80
    Serial.print("Plant_A/");
    Serial.println(cs);
}
