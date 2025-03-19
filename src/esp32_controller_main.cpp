#include <Arduino.h>
#include <Update.h>
#include "PinDef.h"
#include "CarStateManager_ESP.h"
#include "PS4ControlManager.h"
#include "WIFIManager.h"
#include "FirebaseManager.h"

PS4ControlManager *ps4ControlManager;
CarStateManagerESP *carStateManager;
WIFIManager *wifiManager;
FirebaseManager *firebaseManager;

uint8_t newMAC[] = {/*MAC address of my phone*/};
void setup() {
  Serial.begin(9600); 
  // So controller thinks ESP32 is my phone
  if (esp_base_mac_addr_set(newMAC) == ESP_OK) {
      Serial.println("MAC address set successfully");
  } else {
      Serial.println("Failed to set MAC address");
  }

  wifiManager = new WIFIManager(WIFI_TYPE_HOTSPOT); 
  carStateManager = new CarStateManagerESP();
  ps4ControlManager = new PS4ControlManager(carStateManager);
  firebaseManager = new FirebaseManager(carStateManager, DEVICE_TYPE_CONTROLLER);
} 

void loop() {
  ps4ControlManager->checkRightJoyStick();
  ps4ControlManager->checkModePress();
  ps4ControlManager->updatePS4LED();
  firebaseManager->getUID();
  firebaseManager->updateControllerStatus();
  firebaseManager->controllerUpdateCarData();
   
  //Reads only every 1 second
  static unsigned long lastRead = millis();
  if(millis() - lastRead > 1000){
    lastRead = millis();
    firebaseManager->listenStream();
  }
}

