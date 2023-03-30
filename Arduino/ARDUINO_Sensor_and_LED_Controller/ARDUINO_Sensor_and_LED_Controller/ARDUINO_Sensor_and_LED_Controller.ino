#include <CapacitiveSensor.h>
#include <Adafruit_NeoPixel.h>

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

class Plant { //PLANT CLASS. Contains a bunch of information relating to controlling the sensor and the LED strip.
  public:
    //Constructor function for creating the object initially
    Plant (String, int, int, int);

    //Constructors for classes using external libraries
    Adafruit_NeoPixel _strip;
    CapacitiveSensor _plantSensor;
 
    //General object variables
    String _plantName;
    int _triggerThreshold = 2500; 

    //Variables relating to sensor
    int _sensorPinOut;
    int _sensorPinIn;
    long _sensorValue;

    //Variables relating to LED pulse time
    long _pulseStartTime;
    float _pulsePeriod = 250;
    bool _isPulseRunning = false;

    //Variables relating to LEDs
    int _ledPin;
    int _ledCount = 60;
    float _brightness = 0;
    float _brightnessMod;
    int _pulseCount = 2;
    int _rColor = 50;
    int _gColor = 255;
    int _bColor = 50;

    //CLASS FUNCTIONS
    //Functions called initially to define LED strip, and pins for LEDS and Sensor
    setupLED();
    setupSensor();
    
    //Functions used to receive and send sensor data via Serial connection
    sensorRead();
    sensorSerialWrite();

    //Function used to track trigger status.
    isArduinoTriggered();

    //Functions used to control LED behavior
    displayLED();
    runPulse();

    //Support functions to get name, set trigger and display other values.
    String getName();
    setTriggerThreshold(int);
    printObjectValues();
};

Plant::Plant(String plantName, int sensorPinOut, int sensorPinIn, int ledPin):_plantSensor(sensorPinOut, sensorPinIn){
  _plantName = plantName;
  _sensorPinOut = sensorPinOut;
  _sensorPinIn = sensorPinIn;
  _ledPin = ledPin;
  _brightnessMod = (255/_pulsePeriod);
}

Plant::setupLED(){
  _strip.setPin(_ledPin);
  _strip.updateLength(_ledCount);
  _strip.updateType(NEO_GRB + NEO_KHZ800);
  _strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  _strip.clear();
  _strip.setBrightness(_brightness);
  _strip.show();            // Turn OFF all pixels ASAP
  delay(10);
}

String Plant::getName(){
  return _plantName;
}

Plant::setupSensor(){
  _plantSensor = CapacitiveSensor(_sensorPinOut, _sensorPinIn);
  _plantSensor.set_CS_AutocaL_Millis(0);
}

Plant::displayLED(){
  for(int i = 0; i < _ledCount; i++){
    _strip.setPixelColor(i, _rColor, _gColor, _bColor);
  }
  _strip.show();
}

Plant::sensorRead(){
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

Plant::runPulse(){ 
  if(isArduinoTriggered() && !_isPulseRunning){
    _pulseStartTime = millis();
    _isPulseRunning = true;
  }

  if(_isPulseRunning){
    _brightness = _brightness + _brightnessMod;    

    if(_brightness > 250){
      _brightnessMod = _brightnessMod*-1;
    }
    
    if(_brightness < 0){
      _brightnessMod = _brightnessMod*-1;
      _brightness = 0;
      _isPulseRunning = false;
    }
  
    _strip.setBrightness(_brightness);
  
  }
  
  if(millis() > _pulseStartTime + (_pulsePeriod*5)){
    _brightnessMod = _brightnessMod*-1;
    _brightness = 0;
    _strip.setBrightness(_brightness);
    _isPulseRunning = false; 
  }
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



const int PLANT_COUNT = 1; //ADJUST BASED ON NUMBER OF ATTACHED SENSORS!
String plantNames[PLANT_COUNT] = {"Plant_A"}; // "Plant_B", "Plant_C", "Plant_D", "Plant_E", "Plant_F", "Plant_G", "Plant_H", "Plant_I", "Plant_J", "Plant_K"}; //DEFINE PLANT NAMES HERE!

//ARRAY OF PLANT OBJECTS BEING CREATED BASED ON THE PLANT_COUNT NUMBER ABOVE
Plant *plants[PLANT_COUNT];


void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < PLANT_COUNT; i++){
    plants[i] = new Plant(plantNames[i], 30+i, 31+i, 2+i);
    plants[i] -> setupSensor();
    plants[i] -> setupLED();
    plants[i] -> printObjectValues();
  }
}


void loop() {

  long currentMillis = millis(); 

  for(int i = 0; i < PLANT_COUNT; i++){
    plants[i] -> displayLED();
    plants[i] -> runPulse();
      if((currentMillis % 25) == 0){ //Replacement for delay function. By using the Modulo operator on current millis, we run the underlying code block only at times when millis and 25 divided by each other equals a number where the remaining number is 0.
        plants[i] -> sensorRead();
        plants[i] -> sensorSerialWrite();
        plants[i] -> isArduinoTriggered();
        Serial.println();
      }
  }
  receiveSerialData();
}

//Code block that runs through Serial data being send, checks for "/" and converts data into array divided at each "/" symbol-.
void receiveSerialData(){
  if(Serial.read() > 0){
    
    String dataArray[10];
    int r = 0; //Uncertain what this value tracks??
    int t = 0; //this values stores the number of elements to loop through in the new array. The value is incrased each time a new element is added to the array.
    
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
      for(int j = 0; j < PLANT_COUNT; j++){
        if(dataArray[k].equals(plants[j] -> getName())){
          if(dataArray[k+1] != NULL){
            if(dataArray[k+1].toInt() > 0){
              int dataValue = dataArray[k+1].toInt();
              plants[j] -> setTriggerThreshold(dataValue);
            }  
          }       
        }
      }
    } 
    delay(10);
  }
}
