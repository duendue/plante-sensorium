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

int plantATrigger = 25000;

Plant[] plants;

String val;

ControlP5 MyController;

void setup() 
{
  size(1200, 200);
  
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 115200);
  myPort.bufferUntil('\n');
  
  oscP5 = new OscP5(this,12000);
  myBroadcastLocation = new NetAddress("127.0.0.1",4560);
  

  plants = new Plant[1];
  plants[0] = new Plant();
  plants[0].setData("Plant_A", 0, 25000);
  plants[0].display();
  
  MyController = new ControlP5(this);
  
  MyController.addSlider("plantATrigger")
    .setPosition(25,50)
    .setRange(0,40000)
    .setSize(20,100)
    .setValue(plants[0].triggerThreshold)
    ;
}

public class Plant {
  public String plantName;
  public int plantSensorValue;
  public int triggerThreshold;
  
  public void setData(String plantName, int plantSensorValue, int threshold){
    this.plantName = plantName;
    this.plantSensorValue = plantSensorValue;
    this.triggerThreshold = threshold;
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
  text("Sensor value: " + plants[0].plantSensorValue, 15, 175);
  plants[0].setTriggerThreshold(plantATrigger);
  
  serialUpdatePlantSensorData();
  
  for(int i = 0; i < plants.length; i++){
    int note = triggerNote(plants[i].plantSensorValue, plants[i].triggerThreshold);
    sendOSCMessage(note);
  }
  
  for(int i = 0; i < plants.length; i++){
    plants[i].display();
  }
}
