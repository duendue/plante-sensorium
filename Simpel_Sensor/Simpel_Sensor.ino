#include <CapacitiveSensor.h>

CapacitiveSensor cs_2_3 = CapacitiveSensor(2,3); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_4_5 = CapacitiveSensor(4,5); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_6_7 = CapacitiveSensor(6,7); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_8_9 = CapacitiveSensor(8,9); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_10_11 = CapacitiveSensor(10,11); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
CapacitiveSensor cs_12_13 = CapacitiveSensor(12,13); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8

char val;

void setup() {
    Serial.begin(115200);
  cs_2_3.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_4_5.set_CS_AutocaL_Millis(0xFFFFFFFF); 
  cs_6_7.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_8_9.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_10_11.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_12_13.set_CS_AutocaL_Millis(0xFFFFFFFF);
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
  delay(100);
}

void CSread() {
  long cs0 = cs_2_3.capacitiveSensor(30); //a: Sensor resolution is set to 80
  long cs1 = cs_4_5.capacitiveSensor(30); //a: Sensor resolution is set to 80
  long cs2 = cs_6_7.capacitiveSensor(30); //a: Sensor resolution is set to 80
  long cs3 = cs_8_9.capacitiveSensor(30); //a: Sensor resolution is set to 80
  long cs4 = cs_10_11.capacitiveSensor(30); //a: Sensor resolution is set to 80
  long cs5 = cs_12_13.capacitiveSensor(30); //a: Sensor resolution is set to 80

    if(cs0 > 0){
      Serial.print("Plant_A/");
      Serial.print(cs0);
      Serial.print("/");      
    }
    if(cs1 > 0){
      Serial.print("Plant_B/");
      Serial.print(cs1);
      //Serial.print("/");       
    }
    if(cs2 > 0){
      Serial.print("Plant_C/");
      Serial.print(cs2);
      //Serial.print("/");       
    }
    if(cs3 > 0){
      Serial.print("Plant_D/");
      Serial.print(cs3);
      Serial.print("/");       
    }
    if(cs4 > 0){
      Serial.print("Plant_E/");
      Serial.print(cs4);
      Serial.print("/");       
    }
    if(cs5> 0){
      Serial.print("Plant_F/");
      Serial.print(cs5);      
    }
      Serial.println();
}
