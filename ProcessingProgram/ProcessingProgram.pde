import controlP5.*;
import netP5.*;
import oscP5.*;
import processing.serial.*;


OscP5 oscP5;
NetAddress myBroadcastLocation;

Serial myPort;  // Create object from Serial class
String data;      // Data received from the serial port

String dataName = "";
int dataValue = 0;

int initialTriggerThreshold = 25000;

Plant[] plants;

String val;

boolean isContactEstablished = false;

//Slider styling adjustments
int xPosSlider = 50;
int yPosSlider = 50;
int xSizeSlider = 20;
int ySizeSlider = 100;
int startValueSlider = 0;
int endValueSlider = 40000;

//Text styling adjustments
int xPosText = 15;
int yPosText = 175;

ControlP5 MyController;

void setup() 
{
  size(1200, 200);
  MyController = new ControlP5(this);
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 115200);
  myPort.bufferUntil('\n');
  
  oscP5 = new OscP5(this,12000);
  myBroadcastLocation = new NetAddress("127.0.0.1",4560);
  
  plantBuilder();
  
  MyController = new ControlP5(this);

}

public void controlEvent (ControlEvent theEvent){
  for(Plant plant : plants){
    if(plant.id == theEvent.getId()){
      int sliderValue = floor(theEvent.getValue());
      plant.setTriggerThreshold(sliderValue);
      println("Set " + plant.plantName + " trigger threshold to " + plant.triggerThreshold);
    }
  }
}

//Function for building plant objects based on sensor input from arduino. If 1 sensor is attached, this will build 1 plant object. If 10 sensors is attached, this will build 10 plant objects
  
  //While there an initial connection between arduino and processing, but we haven't executed the rest of the program, this function triggers.
  //Data from serial is read and converted to an array of entities split at each '/' symbol
  //Length of the new array is halved in a new array to get number of sensors active on arduino (The arduino sends a name + a sensor value)
  //New array contains the names send from the Arduino. 
  
  //!!! Therefore, arduino is master of plant names. !!!
  
  // create new plant objects, based on number of sensors attached on arduino.
  // designate data of new plant objects, based on naming from arduino
  
void plantBuilder(){
  while(myPort.available() > 0 && !isContactEstablished){
    val = myPort.readStringUntil('\n'); 
      if(val != null){
        val = trim(val); // Fjerner linjer uden data
        String[] dataArray = val.split("/"); 
        int dataArrayCount = dataArray.length;
        
        if(dataArrayCount % 2 == 0){
          //Might consider looping through dataArray to check if ever second entity is a string (ie: if(int(dataArray[i]) == 0)), thereby making an extra check to avoid building faulty plant  objects
          int sensorCount = dataArrayCount/2;
          
          String[] plantNameArray = new String[0];
          
          for(String data : dataArray){
            if(int(data) == 0){
              plantNameArray = append(plantNameArray, data);
            }
          }
          
          println("Number of attached sensors: " + sensorCount);

          plants = new Plant[sensorCount];
          
          for(int i = 0; i < plants.length; i++){
            plants[i] = new Plant();
            plants[i].setData(plantNameArray[i], 0, initialTriggerThreshold, i);
            plants[i].buildSlider();
            plants[i].buildText();
            println("Build plant object: " + plants[i].plantName + " with ID: " + plants[i].id);
            delay(50);
          }

          isContactEstablished = true;
        }else{
          println("Error! Uneven datacount, not possible to build plants!");
          delay(100);
        }
      }
  }
}
//set name, value, threshold, id at creation
public class Plant {
  public String plantName;
  public int plantSensorValue;
  public int triggerThreshold;
  public int id;
  public int xPosSlider = 50;
  public int xPosText = 15;
  
  public void setData(String plantName, int plantSensorValue, int threshold, int id){
    this.plantName = plantName;
    this.plantSensorValue = plantSensorValue;
    this.triggerThreshold = threshold;
    this.id = id;
  }
  
  public void buildSlider(){
    xPosSlider = xPosSlider + (id*xPosSlider);
    MyController.addSlider(this.plantName)
      .setPosition(xPosSlider,yPosSlider)
      .setRange(startValueSlider,endValueSlider)
      .setSize(xSizeSlider,ySizeSlider)
      .setValue(this.triggerThreshold)
      .setId(this.id)
      ;
  }
  
  public void buildText(){
    this.xPosText = this.xPosText + (this.id*this.xPosText);
    text("Sensor value: " + this.plantSensorValue, this.xPosText, yPosText);
  }
  
  public void updateText(){
    this.xPosText = this.xPosText + (this.id*this.xPosText);
    text("Sensor value: " + this.plantSensorValue, this.xPosText, yPosText);
  }
  
  public void setName(String name){
    this.plantName = name;
  }
  
  public void setSensorValue(int sensorValue){
    this.plantSensorValue = sensorValue;
  }
  
  public void setTriggerThreshold(int triggerThreshold){
    this.triggerThreshold = triggerThreshold;
  }
  
  public void display(){
    println("Plant name is: " + plantName + " and plant sensor value is: " + plantSensorValue);
  }
}


public static boolean isNumeric(String str){
  for (char c : str.toCharArray()){
    if(!Character.isDigit(c)) return false;
  }
  return true;
}


public void serialUpdatePlantSensorData(){
  if (myPort.available() > 0) {  // If data is available,
    val = myPort.readStringUntil('\n'); 
    if(val != null){
      val = trim(val); // Fjerner linjer uden data
      String[] dataArray = val.split("/");
      
      for(int i = 0; i < dataArray.length; i++){
        for(int j = 0; j < plants.length; j++){
          if(dataArray[i].equals(plants[j].plantName)){
            int dataValue = int(dataArray[i+1]);
            plants[j].setSensorValue(dataValue);
          }
        }      
      }
    }
  }
}

public void sendOSCMessage(int sensorValue){
  OscMessage myOscMessage = new OscMessage("");
  myOscMessage.add(sensorValue);
  oscP5.send(myOscMessage, myBroadcastLocation);
  delay(100);
}

public int triggerNote(int dataValue, int threshold){
  if(dataValue > 100){
    if (dataValue >= threshold){
      println("Trigger!");
      int soundNote = floor(map(dataValue, threshold, 350000, 60, 100));   
      return soundNote;      
    }
  }
  return 0;
}


void draw(){
  background(55);
  
  for(Plant plant : plants){
    plant.updateText();
    
    int note = triggerNote(plant.plantSensorValue, plant.triggerThreshold);
    sendOSCMessage(note);
  }

  serialUpdatePlantSensorData();
  /*
  for(int i = 0; i < plants.length; i++){
    int note = triggerNote(plants[i].plantSensorValue, plants[i].triggerThreshold);
    sendOSCMessage(note);
  }*/
  /*
  for(int i = 0; i < plants.length; i++){
    plants[i].display();
  }*/
  
}
