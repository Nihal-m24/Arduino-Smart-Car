/**/
#include <Arduino.h>
#include <Update.h>
#include "PinDef.h"
#include "CarStateManager_ESP.h"
#include "SerialCommunicationManager_ESP.h"
#include "FirebaseManager.h"

#include "cameraManager.h"
#include "WIFIManager.h"
#include "FirebaseManager.h"

CarStateManagerESP *carStateManager;
SerialCommunicationManager *serialCommunicationManager;
WIFIManager *wifiManager;
FirebaseManager *firebaseManager;
CameraManager *cameraManager;

void setup() {
  Serial.begin(9600); 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  carStateManager = new CarStateManagerESP();
  serialCommunicationManager = new SerialCommunicationManager(carStateManager);
  wifiManager = new WIFIManager(WIFI_TYPE_HOTSPOT);
  firebaseManager = new FirebaseManager(carStateManager, DEVICE_TYPE_CAMERA);
  cameraManager = new CameraManager();
  cameraManager->initCamera();
  cameraManager->startCameraServer();

}

void loop() {
  firebaseManager->listenStream();
  serialCommunicationManager->sendData();
  cameraManager->handleClient();
  firebaseManager->getUID();
  firebaseManager->updateIPAddress(wifiManager->getIPAddress());

}

