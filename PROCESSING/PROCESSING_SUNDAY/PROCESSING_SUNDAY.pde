import osteele.processing.SerialRecord.*;

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

Plant[] plants;

String val;

boolean isContactEstablished = false;
int dataLengthValidation = 0;

//Slider styling adjustments
int xPosSlider = 50;
int yPosSlider = 100;
int xSizeSlider = 20;
int ySizeSlider = 100;
int startValueSlider = 0;
int endValueSlider = 28000;
int initialTriggerThreshold = 2500;

int maxValue = 28000;
int maxValueMod = 3000;

//Text styling adjustments
int xPosText = 15;
int yPosText = 225;

JSONObject json;

ControlP5 MyController;
ControlP5 PlantSliderController;
ControlP5 MyButtonController;

boolean connectionEstablished = false;

void setup()
{
  size(1300, 300);
  //frameRate(60);

  text("Building plant sensor objects, Please Wait...", 650, 150);
  textAlign(CENTER);

  MyController = new ControlP5(this);
  PlantSliderController = new ControlP5(this);
  MyButtonController = new ControlP5(this);

  setupCommunication();

  MyButtonController.addButton("Refresh")
    .setPosition(15, 15)
    .setSize(100, 20)
    .setId(100)
    ;

  MyButtonController.addButton("Send")
    .setPosition(150, 15)
    .setSize(100, 20)
    .setId(200)
    ;
    
  MyButtonController.addButton("Party Mode")
    .setPosition(300, 15)
    .setSize(100, 20)
    .setId(500)
    ;

  MyButtonController.addButton("Save Data")
    .setPosition(500, 15)
    .setSize(100, 20)
    .setId(300)
    ;

  MyButtonController.addButton("Load Data")
    .setPosition(620, 15)
    .setSize(100, 20)
    .setId(400)
    ;

  plantBuilder();
}

boolean doesFileExist(String filePath) {
  return new File(dataPath(filePath)).exists();
}

public void setupCommunication() {
  oscP5 = new OscP5(this, 12000);
  myBroadcastLocation = new NetAddress("127.0.0.1", 4560);

  println("-----------------------------------");
  delay(1000);

  String serialPortName = SerialUtils.findArduinoPort();
  boolean connectionEstablished = false;

  println("-----------------------------------");
  delay(1000);

  while (serialPortName == null) {
    if (Serial.list().length > 2) {
      println("-----------------------------------");
      println("Arduino registered! Setting up Serial Connection!");
      println("-----------------------------------");
      delay(2000);
      serialPortName = Serial.list()[2];
      myPort = new Serial(this, serialPortName, 115200);
      myPort.bufferUntil('\n');
    } else {
      println("NO Arduino registered in Serial connection. Please attach an arduino to a valid USB port.");
      delay(500);
    }
  }
  while (!connectionEstablished) {
    if (myPort.available() >= 0) {
      val = myPort.readStringUntil('\n');
      if (val != null) {
        val = trim(val);
        if (int(val) > 0 && int(val) < 20) {
          myPort.clear();
          connectionEstablished = true;
          dataLengthValidation = int(val);
          myPort.write("GO");
          println("Connection made! Handshaking with Arduino!");
          println("Amount of sensors registered to connect to: " + dataLengthValidation);
          delay(2000);
        }
      } else {
        println("Arduino sending empty messages.");
        println(val);
        delay(500);
      }
    } else {
      println("No messages received from connected Arduino. Verify that right program is running on Arduino.");
      val = trim(val);
      println(val);
      delay(500);
    }
  }
}

public void Refresh() {
  clearAll();
  plantBuilder();
}

public void Send() {
  if (plants != null) {
    myPort.write("000000/");
    for (Plant plant : plants) {
      myPort.write(plant.plantName + "/" + plant.triggerThreshold + "/");
      delay(10);
    }
  }
}


public void CyclePlants() {
  for (Plant plant : plants) {
    println(plant);
  }
}

public void clearAll() {
  if (plants != null) {
    for (Plant plant : plants) {
      plant.removeSlider();
      delay(20);
    }
  }

  plants = null;
  isContactEstablished = false;
  background(55);
  delay(50);
}

public void controlEvent (ControlEvent theEvent) {
  if (plants != null) {
    for (Plant plant : plants) {
      if (plant != null && plant.id == theEvent.getId()) {
        int sliderValue = floor(theEvent.getValue());
        plant.setTriggerThreshold(sliderValue);
        //println("Set " + plant.plantName + " trigger threshold to " + plant.triggerThreshold);
        //myPort.write(plant.plantName + "/" + plant.triggerThreshold + "/");
      }
    }
  }

  if (theEvent.getId() == 200 && plants != null) {
    for (Plant plant : plants) {
      myPort.write(plant.plantName + "/" + plant.triggerThreshold + "/");
    }
  }

  if (theEvent.getId() == 300) {
    saveData();
  }

  if (theEvent.getId() == 400) {
    loadData();
    Send();
  }
  
  if(theEvent.getId() == 500) {
    partyMode();
  }
}

JSONObject plantsJSON;
JSONArray plantsData;

void createData() {
  plantsJSON = new JSONObject();
  plantsData = new JSONArray();
  plantsJSON.setJSONArray("Plants", plantsData);

  for (int i = 0; i < plants.length; i++) {
    JSONObject newPlant = new JSONObject();
    newPlant.setInt("Plant Index", i);
    newPlant.setString("Plant Name", plants[i].plantName);
    newPlant.setInt("Trigger Threshold", initialTriggerThreshold);

    plantsData.setJSONObject(i, newPlant);
  }
  saveJSONObject(plantsJSON, "data/data.json");
}

void saveData() {
  plantsJSON = new JSONObject();
  plantsData = new JSONArray();
  plantsJSON.setJSONArray("Plants", plantsData);

  for (int i = 0; i < plants.length; i++) {
    JSONObject newPlant = new JSONObject();
    newPlant.setInt("Plant Index", i);
    newPlant.setString("Plant Name", plants[i].plantName);
    newPlant.setInt("Trigger Threshold", plants[i].triggerThreshold);

    plantsData.setJSONObject(i, newPlant);
  }
  saveJSONObject(plantsJSON, "data/data.json");
}

void loadData() {
  JSONObject json = loadJSONObject("data.json");
  JSONArray plantsArrayJSON = json.getJSONArray("Plants");

  for (int i = 0; i < plants.length; i++) {
    JSONObject plantJSON = plantsArrayJSON.getJSONObject(i);
    int plantI = plantJSON.getInt("Plant Index");
    String plantName = plantJSON.getString("Plant Name");
    int plantTrigger = plantJSON.getInt("Trigger Threshold");

    setSliderTrigger(plantName, plantTrigger);
    plants[i].setTriggerThreshold(plantTrigger);
  }
}

public void setSliderTrigger(String sliderName, int newTriggerValue) {
  PlantSliderController.getController(sliderName).setValue(newTriggerValue);
}

//Function for building plant objects based on sensor input from arduino. If 1 sensor is attached, this will build 1 plant object. If 10 sensors is attached, this will build 10 plant objects

//While there an initial connection between arduino and processing, but we haven't executed the rest of the program, this function triggers.
//Data from serial is read and converted to an array of entities split at each '/' symbol
//Length of the new array is halved in a new array to get number of sensors active on arduino (The arduino sends a name + a sensor value)
//New array contains the names send from the Arduino.

//!!! Therefore, arduino is master of plant names. !!!

// create new plant objects, based on number of sensors attached on arduino.
// designate data of new plant objects, based on naming from arduino

void plantBuilder() {
  boolean buildObjects = false;

  delay(50);

  while (!buildObjects) {
    val = myPort.readStringUntil('\n');
    val = trim(val);
    println(val);

    if (val != null) {
      String[] dataArray = val.split("/");
      int dataArrayCount = dataArray.length;
      println(dataArrayCount);

      if (dataArrayCount % 2 == 0 && (dataArrayCount / 2) == dataLengthValidation) {


        //Might consider looping through dataArray to check if ever second entity is a string (ie: if(int(dataArray[i]) == 0)), thereby making an extra check to avoid building faulty plant  objects
        int sensorCount = dataArrayCount/2;

        int[] plantValueArray = new int[0];

        for (String data : dataArray) {
          if (int(data) != 0) {
            plantValueArray = append(plantValueArray, int(data));
          }
        }
        int plantValueArrayCount = plantValueArray.length;


        //outerLoop:
        String[] plantNameArray = new String[0];

        for (String data : dataArray) {
          if (int(data) == 0) {
            for (String name : plantNameArray) {
              if (name == data) {
                //break outerLoop;
              }
            }
            plantNameArray = append(plantNameArray, data);
          }
        }

        println("Number of attached sensors: " + sensorCount);

        plants = new Plant[sensorCount];

        for (int i = 0; i < plants.length; i++) {
          plants[i] = new Plant();
          plants[i].setData(plantNameArray[i], 0, initialTriggerThreshold, i);
          plants[i].buildSlider();
          plants[i].buildText();
          println("Build plant object: " + plants[i].plantName + " with ID: " + plants[i].id);
          delay(50);
        }
        buildObjects = true;
        delay(10);
        myPort.write("Finished Building");

        isContactEstablished = true;
      } else {
        println("Data received, but incomplete / missing points.");
        delay(50);
      }
    } else {
      myPort.write("A");

      println("No data received.");
      delay(10);
    }
    delay(100);
  }
}




//set name, value, threshold, id at creation
public class Plant {
  public String plantName;
  public int plantSensorValue;
  public int triggerThreshold;
  public int maxValue;
  public int id;
  public int xPos = 100;
  public int xPosText = 100;
  public int xPosMod = 100;


  public void setData(String plantName, int plantSensorValue, int threshold, int id) {
    this.plantName = plantName;
    this.plantSensorValue = plantSensorValue;
    this.triggerThreshold = threshold;
    this.id = id;
    this.maxValue = triggerThreshold + maxValueMod;



    this.xPos = this.xPos + (this.id*this.xPosMod);
  }

  public void buildSlider() {
    PlantSliderController.addSlider(this.plantName)
      .setPosition(xPos, yPosSlider)
      .setRange(startValueSlider, endValueSlider)
      .setSize(xSizeSlider, ySizeSlider)
      .setValue(this.triggerThreshold)
      .setId(this.id)
      .setNumberOfTickMarks(500)
      .showTickMarks(false)
      .snapToTickMarks(true)
      .setSliderMode(1)
      ;
  }

  public void setMaxValue() {
    this.maxValue = this.triggerThreshold + maxValueMod;
  }

  public void removeSlider() {
    PlantSliderController.remove(this.plantName);
  }

  public void buildText() {
    text("Sensor value: " + this.plantSensorValue, this.xPos, yPosText);
    textAlign(CENTER);
  }

  public void updateText() {
    text("Sensor value: " + this.plantSensorValue, this.xPos, yPosText);
    textAlign(CENTER);
  }

  public void triggerText() {
    text("Triggered!", this.xPos, 90);
    textAlign(CENTER);
  }

  public void setName(String name) {
    this.plantName = name;
  }

  public void setSensorValue(int sensorValue) {
    this.plantSensorValue = sensorValue;
  }

  public void setTriggerThreshold(int triggerThreshold) {
    this.triggerThreshold = triggerThreshold;
    this.setMaxValue();
  }

  public void display() {
    println("Plant name is: " + plantName + " and plant sensor value is: " + plantSensorValue);
  }
}


public static boolean isNumeric(String str) {
  for (char c : str.toCharArray()) {
    if (!Character.isDigit(c)) return false;
  }
  return true;
}

public void partyMode(){
  for(int i = 0; i < plants.length; i++){
    println("Playing scale from: " + plants[i].plantName);
    for(int j = 0; j < 20; j++){
      sendOSCMessage(plants[i].plantName, j);
      delay(300);
    }
    delay(1000);
  }
}

public void serialUpdatePlantSensorData() {
  if (myPort.available() > 0) {  // If data is available,
    val = myPort.readStringUntil('\n');
    if (val != null) {
      val = trim(val); // Fjerner linjer uden data
      String[] dataArray = val.split("/");
      int dataArrayCount = dataArray.length;

      if (dataArray.length > 1 && (dataArrayCount / 2) == dataLengthValidation && dataArrayCount % 2 == 0) {
        for (int i = 0; i < dataArray.length; i++) {
          for (int j = 0; j < plants.length; j++) {
            if (dataArray[i].equals(plants[j].plantName)) {
              int dataValue = int(dataArray[i+1]);
              if (plants[j].plantSensorValue == dataValue) {
                break;
              } else {
                plants[j].setSensorValue(dataValue);
              }
            }
          }
        }
      }
    }
  }
}

public void sendOSCMessage(String plantName, int sensorValue) {
  OscMessage myOscMessage = new OscMessage(plantName);
  myOscMessage.add(sensorValue);
  oscP5.send(myOscMessage, myBroadcastLocation);
}

public int triggerNote(Plant plantObj, int dataValue, int threshold) {
  if (dataValue > 100) {
    if (dataValue >= threshold) {
      plantObj.triggerText();
      dataValue = constrain(dataValue, 0, maxValue);
      int soundNote = floor(map(dataValue, threshold, maxValue, 0, 20));
      return soundNote;
    }
  }
  return 0;
}

public int valueConverter(int dataValue, int threshold, int plantMaxValue) {
  dataValue = constrain(dataValue, threshold, plantMaxValue);
  int soundNote = floor(map(dataValue, threshold, plantMaxValue, 7, 15));
  /*if(soundNote > 19){
   soundNote = 19;
   }*/
  return soundNote;
}



//if trigger threshold
//convert value to note
//send message

void draw() {
  background(55);
  if (plants != null) {
    for (Plant plant : plants) {
      plant.updateText();
      if (plant.plantSensorValue > plant.triggerThreshold) {
        plant.triggerText();
        float currentMillis = millis();
        if (currentMillis % 10 == 0) {
          int note = valueConverter(plant.plantSensorValue, plant.triggerThreshold, plant.maxValue);
          sendOSCMessage(plant.plantName, note);
        }
      }
    }
    serialUpdatePlantSensorData();
  }
}
