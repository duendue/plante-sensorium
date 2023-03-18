#include <CapacitiveSensor.h>

CapacitiveSensor cs_2_3 = CapacitiveSensor(2,3); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_4_5 = CapacitiveSensor(4,5); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_6_7 = CapacitiveSensor(6,7); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_8_9 = CapacitiveSensor(8,9); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_10_11 = CapacitiveSensor(10,11); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8


char val;

void setup() {
    Serial.begin(115200);
  cs_2_3.set_CS_AutocaL_Millis(0);
  cs_4_5.set_CS_AutocaL_Millis(0); 
  cs_6_7.set_CS_AutocaL_Millis(0);
  cs_8_9.set_CS_AutocaL_Millis(0);
  cs_10_11.set_CS_AutocaL_Millis(0);
  
    //establishContact();
    
}

void establishContact(){
  while (Serial.available() <= 0){
    Serial.println("A");
    delay(300);
  }
}
void loop() {
  
  CSread();
  delay(50);
}

void CSread() {
  long cs0 = cs_2_3.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
  long cs1 = cs_4_5.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
  long cs2 = cs_6_7.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
  long cs3 = cs_8_9.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
  long cs4 = cs_10_11.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    
    Serial.print("Plant_A/");
    Serial.print(cs0);
    Serial.print("/");    
    Serial.print("Plant_B/");
    Serial.print(cs1);
    Serial.print("/");  
    Serial.print("Plant_C/");
    Serial.print(cs2);
    Serial.print("/"); 
    Serial.print("Plant_D/");
    Serial.print(cs3);
    Serial.print("/"); 
    Serial.print("Plant_E/");
    Serial.print(cs4);
    Serial.println();
}
