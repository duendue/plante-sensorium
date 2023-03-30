#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

CapacitiveSensor Plant_A_Sensor = CapacitiveSensor(30, 31);
CapacitiveSensor Plant_B_Sensor = CapacitiveSensor(32, 33);
CapacitiveSensor Plant_C_Sensor = CapacitiveSensor(34, 35);
CapacitiveSensor Plant_D_Sensor = CapacitiveSensor(36, 37);
CapacitiveSensor Plant_E_Sensor = CapacitiveSensor(38, 39);
CapacitiveSensor Plant_F_Sensor = CapacitiveSensor(40, 41);
CapacitiveSensor Plant_G_Sensor = CapacitiveSensor(42, 43);
CapacitiveSensor Plant_H_Sensor = CapacitiveSensor(44, 45);
CapacitiveSensor Plant_I_Sensor = CapacitiveSensor(46, 47);
CapacitiveSensor Plant_J_Sensor = CapacitiveSensor(48, 49);
CapacitiveSensor Plant_K_Sensor = CapacitiveSensor(50, 51);
CapacitiveSensor Plant_L_Sensor = CapacitiveSensor(52, 53);

Adafruit_NeoPixel Plant_A_LEDstrip(60, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_B_LEDstrip(60, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_C_LEDstrip(60, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_D_LEDstrip(60, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_E_LEDstrip(60, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_F_LEDstrip(60, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_G_LEDstrip(60, 8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_H_LEDstrip(60, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_I_LEDstrip(60, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_J_LEDstrip(60, 11, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_K_LEDstrip(60, 12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Plant_L_LEDstrip(60, 13, NEO_GRB + NEO_KHZ800);

int senseDelay = 50;

void setup() {
  Serial.begin(115200);

  Plant_A_Sensor.set_CS_AutocaL_Millis(0);
  Plant_B_Sensor.set_CS_AutocaL_Millis(0);
  Plant_C_Sensor.set_CS_AutocaL_Millis(0);
  Plant_D_Sensor.set_CS_AutocaL_Millis(0);
  Plant_E_Sensor.set_CS_AutocaL_Millis(0);
  Plant_F_Sensor.set_CS_AutocaL_Millis(0);
  Plant_G_Sensor.set_CS_AutocaL_Millis(0);
  Plant_H_Sensor.set_CS_AutocaL_Millis(0);
  Plant_I_Sensor.set_CS_AutocaL_Millis(0);
  Plant_J_Sensor.set_CS_AutocaL_Millis(0);
  Plant_K_Sensor.set_CS_AutocaL_Millis(0);
  Plant_L_Sensor.set_CS_AutocaL_Millis(0);

  Plant_A_LEDstrip.begin();
  Plant_B_LEDstrip.begin();
  Plant_C_LEDstrip.begin();
  Plant_D_LEDstrip.begin();
  Plant_E_LEDstrip.begin();
  Plant_F_LEDstrip.begin();
  Plant_G_LEDstrip.begin();
  Plant_H_LEDstrip.begin();
  Plant_I_LEDstrip.begin();
  Plant_J_LEDstrip.begin();
  Plant_K_LEDstrip.begin();
  Plant_L_LEDstrip.begin();

  Plant_A_LEDstrip.show();
  Plant_B_LEDstrip.show();
  Plant_C_LEDstrip.show();
  Plant_D_LEDstrip.show();
  Plant_E_LEDstrip.show();
  Plant_F_LEDstrip.show();
  Plant_G_LEDstrip.show();
  Plant_H_LEDstrip.show();
  Plant_I_LEDstrip.show();
  Plant_J_LEDstrip.show();
  Plant_K_LEDstrip.show();
  Plant_L_LEDstrip.show();
}

void CSread(int senseDelay) {
  long currentMillis = millis();
  if((currentMillis % senseDelay) == 0){
    long Plant_A_Reading = Plant_A_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_B_Reading = Plant_B_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_C_Reading = Plant_C_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_D_Reading = Plant_D_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_E_Reading = Plant_E_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_F_Reading = Plant_F_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_G_Reading = Plant_G_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_H_Reading = Plant_H_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_I_Reading = Plant_I_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_J_Reading = Plant_J_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_K_Reading = Plant_K_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
    long Plant_L_Reading = Plant_L_Sensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80

    
    Serial.print("Plant_A/");
    Serial.print(Plant_A_Reading);
    Serial.print("/");
    Serial.print("Plant_B/");
    Serial.print(Plant_B_Reading);
    Serial.print("/");
    Serial.print("Plant_C/");
    Serial.print(Plant_C_Reading);
    Serial.print("/");
    Serial.print("Plant_D/");
    Serial.print(Plant_D_Reading);
    Serial.print("/");
    Serial.print("Plant_E/");
    Serial.print(Plant_E_Reading);
    Serial.print("/");
    Serial.print("Plant_F/");
    Serial.print(Plant_F_Reading);
    Serial.print("/");
    Serial.print("Plant_G/");
    Serial.print(Plant_G_Reading);
    Serial.print("/");
    Serial.print("Plant_H/");
    Serial.print(Plant_H_Reading);
    Serial.print("/");
    Serial.print("Plant_I/");
    Serial.print(Plant_I_Reading);
    Serial.print("/");
    Serial.print("Plant_J/");
    Serial.print(Plant_J_Reading);
    Serial.print("/");
    Serial.print("Plant_K/");
    Serial.print(Plant_K_Reading);
    Serial.print("/");
    Serial.print("Plant_L/");
    Serial.print(Plant_L_Reading);
    Serial.println(); 
  }
}


// loop() function -- runs repeatedly as long as board is on ---------------

String triggerValue = "2500";



void loop() {
 /* for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 75, 75, 255);
  }

  pulse();
  strip.show(); */

  plant_a.displayLED();

  long currentMillis = millis();
  if((currentMillis % 30) == 0){
    plant_a.sensorRead();
    plant_a.sensorSerialWrite();
    plant_a.isArduinoTriggered();
    Serial.println();
  }

  //Reads sensor pins. Input value specifies the delay between each reading. Delay is required as it limits overflow of data.
  //CSread(30);

  receiveSerialData();

}

//Code block that runs through Serial data being send, checks for "/" and converts data into array divided at each "/" symbol-.
void receiveSerialData(){
  if(Serial.read() > 0){
    
    String dataArray[10];
    int r = 0;
    int t = 0;
    
    String dataString = Serial.readString();
    dataString.trim();
    for(int i = 0; i < dataString.length(); i++){
      if(dataString[i] == '/'){
        if(i-r > 1){
          dataArray[t] = dataString.substring(r, i);
          t++;
        }
        r = (i+1);
      }
    }

    
    for(int k = 0; k <= t; k++){
      if(dataArray[k].equals("Plant_A")){
        if(dataArray[k+1] != NULL){
          if(dataArray[k+1].toInt() > 0){
            int dataValue = dataArray[k+1].toInt();
            plant_a.setTriggerThreshold(dataValue);
            plant_a.printObjectValues();
          }  
        }       
      }
    } 

    delay(10);
  }
}

void updateStrip(){
  
}

bool runningPulse = false;
/*
void triggerPulse(){
  runningPulse = true;
  strip.clear();
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 75, 75, 255);
  }
  
  strip.setBrightness(0);

  strip.show();   
}

void pulse(){
  if(millis() > timeNow + period){
    brightnessMod = brightnessMod*-1;
    timeNow = millis();
  }
  //Serial.println(brightness);
  brightness = brightness + brightnessMod;
  if(brightness > 255){
    brightness = 255;
  }
  if(brightness < 0){
    brightness = 0;
  }
  plant_a._strip.setBrightness(brightness);
}*/
