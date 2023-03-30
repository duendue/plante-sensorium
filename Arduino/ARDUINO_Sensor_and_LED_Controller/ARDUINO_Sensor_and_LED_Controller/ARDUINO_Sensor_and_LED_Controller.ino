#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

//#include "Plant.h"

class Plant {
  public:
    Plant (String, int, int, int);

    long _sensorValue;
    String _plantName;
    int _sensorPinOut;
    int _sensorPinIn;
    int _ledPin;
    int _triggerThreshold = 2500; 
    
    Adafruit_NeoPixel _strip;
    
    float _period = 1500;
    bool _runningPulse = false;
    
    setTriggerThreshold(int);
    printObjectValues();
    sensorRead();
    sensorSerialWrite();
    isArduinoTriggered();
    setupLED();
    displayLED();
};

Plant::Plant(String plantName, int sensorPinOut, int sensorPinIn, int ledPin){
  _plantName = plantName;
  _sensorPinOut = sensorPinOut;
  _sensorPinIn = sensorPinIn;
  _ledPin = ledPin;


}

Plant::setupLED(){
  Adafruit_NeoPixel _strip(60, _ledPin, NEO_GRB + NEO_KHZ800);
  _strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  _strip.show();            // Turn OFF all pixels ASAP
  _strip.setBrightness(255);
}

Plant::displayLED(){
  for(int i = 0; i < _strip.numPixels(); i++){
    _strip.setPixelColor(i, 75, 75, 255);
  }
  _strip.show();
}

Plant::sensorRead(){
  CapacitiveSensor _plantSensor = CapacitiveSensor(_sensorPinOut, _sensorPinIn);
  _plantSensor.set_CS_AutocaL_Millis(0);
  _sensorValue = _plantSensor.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80
}

Plant::sensorSerialWrite(){
  Serial.print(_plantName);
  Serial.print("/");
  Serial.print(_sensorValue);
}

Plant::setTriggerThreshold(int triggerThreshold){
  _triggerThreshold = triggerThreshold;
}

Plant::isArduinoTriggered(){
  if(_sensorValue > _triggerThreshold && Serial.read() < 1){
    return true;
  }else{
    return false;
  }
}

Plant::printObjectValues(){
  Serial.println(_plantName);
  Serial.println(_sensorPinOut);
  Serial.println(_sensorPinIn);
  Serial.println(_ledPin);
  Serial.println(_sensorValue);
  Serial.println(_triggerThreshold);
}

//CapacitiveSensor csPins1 = CapacitiveSensor(30,31); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8

// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 96

// Declare our NeoPixel strip object:
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

long timeNow = 0;
float period = 1500;
long currentMillis = 0;

float brightness = 0;
float brightnessMod = 0;


Plant plant_a("Plant_A", 30, 31, 6);

void setup() {
  Serial.begin(115200);

  plant_a.printObjectValues();
  plant_a.setupLED();
  
  //csPins1.set_CS_AutocaL_Millis(0);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
/*
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
*/
  brightnessMod = 255/period;
  Serial.println(brightnessMod);
}

/*
void CSread(int senseDelay) {
  long currentMillis = millis();
  if((currentMillis % senseDelay) == 0){
    long cs1 = csPins1.capacitiveSensorRaw(30); //a: Sensor resolution is set to 80

    
    Serial.print("Plant_A/");
    Serial.print(cs1);
    Serial.println(); 
  }
}*/


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
