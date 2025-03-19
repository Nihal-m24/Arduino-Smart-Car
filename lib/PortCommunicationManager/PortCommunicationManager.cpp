#include <PortCommunicationManager.h>

PortCommunicationManager::PortCommunicationManager(CarStateManager * carStateManager, DriveManager* driveManager,  VoltageManager* voltageManager){
    this->carStateManager = carStateManager;
    this->driveManager = driveManager;
    this->voltageManager = voltageManager;

    syncRequestReceived = false;
    syncRequestSent = false;
    heartBeat = 0;
    lastSendTime = 0;
    lastReceiveTime = 0;

    forwardSpeed = 0;
    backwardSpeed = 0;
    rightSpeed = 0;
    leftSpeed = 0;
    directionOfCar = "stop";
    directionsToFollow = "1";
    carState = 0;
}

void PortCommunicationManager::recieveData(){    
    static String receivedData = "";
    if (Serial.available()) {
        char c = Serial.read(); 

        if (c == '}') { 
            processData(receivedData);
            receivedData = "";
        } else {
            receivedData += c;
        }
    }
}

void PortCommunicationManager::sendData(){
    if(carState != carStateManager->getCarState()){
        Serial.print("{");
        carState = (carStateManager->getCarState());
        Serial.print("carState=");
        Serial.print(carState);
        Serial.print(";");
        Serial.print("}");
    }
}


void PortCommunicationManager::processData(String serialPortData){
    int carStatePos = serialPortData.indexOf("carState=");
    if (carStatePos != -1) {
        carState = serialPortData.substring(carStatePos + 9).toInt();
        carStateManager->changeCarState(carState);
    } else {
        carStateManager->changeCarState(MODE_RECIEVE_TEST_RED);
    }


    //new stuff
     
   // carState = getValue(serialPortData, "carState").toInt();
   if(carState == MODE_REMOTE_CONTROL){
        forwardSpeed = getValue(serialPortData, "forwardSpeed").toInt();
        backwardSpeed = getValue(serialPortData, "backwardSpeed").toInt();
        rightSpeed = getValue(serialPortData, "rightSpeed").toInt();
        leftSpeed = getValue(serialPortData, "leftSpeed").toInt();
        directionOfCar = getValue(serialPortData, "carDirection");
   } else if (carState == MODE_INPUT_DIRECTIONS) {
        directionsToFollow = getValue(serialPortData, "directionsToFollow");
        carStateManager->updateDirectionsToFollow(directionsToFollow);
   }

   /* 
    static unsigned long lastPrinted = millis();
    if(millis() - lastPrinted > 500){
        lastPrinted = millis();
        Serial.println("Received Data:");
        Serial.print("Car State: "); Serial.println(carState);
        Serial.print("Forward Speed: "); Serial.println(forwardSpeed);
        Serial.print("Backward Speed: "); Serial.println(backwardSpeed);
        Serial.print("Right Speed: "); Serial.println(rightSpeed);
        Serial.print("Left Speed: "); Serial.println(leftSpeed);
        Serial.print("Car Direction: "); Serial.println(directionOfCar);
        Serial.println();
    }
    */

    if(directionOfCar == "Forward"){
        carStateManager->updateSpeed(directionOfCar, forwardSpeed, 0);
    } else if (directionOfCar == "Backward"){
        carStateManager->updateSpeed(directionOfCar, backwardSpeed, 0);
    } else if (directionOfCar == "Right"){
        carStateManager->updateSpeed(directionOfCar, 0, rightSpeed);
    } else if (directionOfCar == "Left"){
        carStateManager->updateSpeed(directionOfCar, 0, leftSpeed);
    } else if (directionOfCar == "ForwardLeft"){
        carStateManager->updateSpeed(directionOfCar, forwardSpeed, leftSpeed);
    } else if (directionOfCar == "ForwardRight"){
        carStateManager->updateSpeed(directionOfCar, forwardSpeed, rightSpeed);
    } else if (directionOfCar == "BackwardLeft"){
        carStateManager->updateSpeed(directionOfCar, backwardSpeed, leftSpeed);
    } else if (directionOfCar == "BackwardRight"){
        carStateManager->updateSpeed(directionOfCar, backwardSpeed, rightSpeed);
    } else if (directionOfCar == "Stop"){
        carStateManager->updateSpeed(directionOfCar, 0, 0);
    } else {
        carStateManager->updateSpeed("Stop", 0, 0);
    }
   
}

String PortCommunicationManager::getValue(String data, String key) {
    int start = data.indexOf(key + "=");
    if (start == -1) return "";

    start += key.length() + 1; // Move index to the value
    int end = data.indexOf(";", start);
    if (end == -1) end = data.length(); // Handle last value case

    return data.substring(start, end);
}