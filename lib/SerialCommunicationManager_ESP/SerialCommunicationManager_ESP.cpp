#include "SerialCommunicationManager_ESP.h"

SerialCommunicationManager::SerialCommunicationManager(CarStateManagerESP *CarStateManager){
    this->carStateManager = CarStateManager;
    directionOfCar = "stop";
    forwardSpeed = 0;
    backwardSpeed = 0;
    rightSpeed = 0;
    leftSpeed = 0;
    carState = -1;
    directionsToFollow = "0";
    syncRequestReceived = false;
}

void SerialCommunicationManager::receiveData(){
    // static String receivedData = "";
    // if (Serial2.available()) {
        // char c = Serial2.read(); 

        // if (c == '}') { 
            // processData(receivedData);
            // receivedData = "";
        // } else {
            // receivedData += c;
        // }
    // }
}

void SerialCommunicationManager::sendData(){
    static unsigned long lastSent = millis();

    if(millis() - lastSent > 100){
        lastSent = millis();
       if(carState != carStateManager->getCarState()){
            carState = carStateManager->getCarState();
            Serial2.print("{");
            Serial2.print("carState="); 
            Serial2.print(carState);
            Serial2.print(";");
            Serial2.println("}");
        }

        if(carState == MODE_REMOTE_CONTROL){
            carState = carStateManager->getCarState();
            Serial2.print("{");

            Serial2.print("carState="); 
            Serial2.print(carState);
            Serial2.print(";");

            Serial2.print("forwardSpeed=");
            Serial2.print(forwardSpeed = carStateManager->getSpeed("Forward"));
            Serial2.print(";");

            Serial2.print("backwardSpeed=");
            Serial2.print(backwardSpeed = carStateManager->getSpeed("Backward"));
            Serial2.print(";");

            Serial2.print("rightSpeed=");
            Serial2.print(rightSpeed = carStateManager->getSpeed("Right"));
            Serial2.print(";");

            Serial2.print("leftSpeed=");
            Serial2.print(leftSpeed = carStateManager->getSpeed("Left"));
            Serial2.print(";");

            Serial2.print("carDirection=");
            Serial2.print(directionOfCar = carStateManager->getDirection());
            Serial2.print(";");
            
            Serial2.println("}");
        } else if (carState == MODE_INPUT_DIRECTIONS) {
            carState = carStateManager->getCarState();
            Serial2.print("{");

            Serial2.print("carState="); 
            Serial2.print(carState);
            Serial2.print(";");
            
            Serial2.print("directionsToFollow=");
            Serial2.print(directionsToFollow = carStateManager->getDirectionsToFollow());
            Serial2.print(";");
            Serial2.println("}");
        }
       
    }
         
}

void SerialCommunicationManager::processData(String serialPortData){
    
    
}