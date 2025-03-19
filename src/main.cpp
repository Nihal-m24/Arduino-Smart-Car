#include <Arduino.h>
#include "ModeManager.h"

#include <FastLED.h>
#include "PinDef.h"
#include "LEDManager.h"
#include "DriveManager.h"
#include "ShieldManager.h"
#include "CarStateManager.h"
#include "PortCommunicationManager.h"
#include "VoltageManager.h"
#include "ServoManager.h"
#include "ultrasonicManager.h"
#include "IRTrackingManager.h"
#include "test.h" 
#include "RemoteDriveManager.h"

//MANAGERS
LEDManager *ledManager;
DriveManager *driveManger;
ShieldManager *shieldManager;
CarStateManager *carStateManager;
VoltageManager *voltageManager;
PortCommunicationManager *portCommunicationManager;
Test *testObject;
IRTrackingManager *tracker;
RemoteDriveManager *remoteDriveManager;
UltrasonicManager *ultrasonicManager;

void setup() {
  Serial.begin(9600); //FOR COMMUNICATING WITH THE ESP32
  ledManager = new LEDManager();
  driveManger = new DriveManager();
  carStateManager = new CarStateManager(driveManger);
  shieldManager = new ShieldManager(carStateManager);
  voltageManager = new VoltageManager();
  portCommunicationManager = new PortCommunicationManager(carStateManager, driveManger, voltageManager);
  ultrasonicManager = new UltrasonicManager();
  remoteDriveManager = new RemoteDriveManager(carStateManager, driveManger);
  tracker = new IRTrackingManager();
  testObject = new Test(driveManger, tracker);
  ledManager->setLEDColor(0, 255, 0);
}

void setLED(){
  switch (carStateManager->getCarState())
  {
  case MODE_REMOTE_CONTROL:
    ledManager->setLEDColor(255, 127, 0); //Orange
    break;

  case MODE_LINE_FOLLOW:
    ledManager->setLEDColor(0, 255, 255); //Cyan
    break;

  case MODE_SELF_DRIVING:
    ledManager->setLEDColor(255, 0, 255); //Purple
    break;

  case MODE_INPUT_DIRECTIONS:
    ledManager->setLEDColor(175, 200, 3); //Yellow
    break;

  case MODE_RECIEVE_TEST_RED:
    ledManager->setLEDColor(255, 0, 0); //Red
    break;
    
  default:
    ledManager->setLEDColor(0, 255, 0); // Green
    break;
  }
}

void loop() {
  shieldManager->detectPress();
  int carState = carStateManager->getCarState();
  String newDirections = "";
  portCommunicationManager->recieveData();
  portCommunicationManager->sendData();
  
  if(carState == MODE_REMOTE_CONTROL){
    remoteDriveManager->driveCar();
  } else if(carState == MODE_LINE_FOLLOW) {
    //Line Following
    testObject->Init();
    testObject->getIRSensorData();
    testObject->testDrive();
  } else if (carState == MODE_SELF_DRIVING){
    //Self Driving
    ultrasonicManager->UltrasonicMode();
  } else if (carState == MODE_INPUT_DIRECTIONS){
    /* INPUT_DIRECTIONS_MODE */
    testObject->updateDirections(carStateManager->getDirectionsToFollow());

  } else {
    driveManger->stop();
  }

  setLED();
}
