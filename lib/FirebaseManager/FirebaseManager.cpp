#include "FirebaseManager.h"
#include <addons/TokenHelper.h>
#include "addons/RTDBHelper.h"

FirebaseManager::FirebaseManager(CarStateManagerESP *carStateManager, int type){
    this->carStateManager = carStateManager;
    config.database_url = DATABASE_URL;
    config.api_key = API_KEY;
    config.token_status_callback = tokenStatusCallback;
    config.max_token_generation_retry = 5;
    deviceType = type;
    if(deviceType == DEVICE_TYPE_CONTROLLER){
        auth.user.email = Controller_Email;
        auth.user.password = Controller_Passwrod;
    } else {
        auth.user.email = Car_Email;
        auth.user.password = Car_Password;
    }
   
    Firebase.reconnectNetwork(true);
    Firebase.begin(&config, &auth);
    Serial.println("Config sucessful");
    lastTime = 0;
    pathToData = "cars/" DEVICE_ID;
}

void FirebaseManager::getUID(){
    static unsigned long lastChecked = millis();
    static bool wasUpdated = false;

    if(wasUpdated){
       return;
    }
    if(millis() - lastChecked > 500){
        if(auth.token.uid == ""){
            lastChecked = millis();
            Serial.print("No Update");
            return;
        } else {
            authUID = auth.token.uid.c_str();
            Serial.print("Recieved ID: ");
            Serial.print(authUID);
            wasUpdated = true;
        }
    }
}

void FirebaseManager::updateControllerStatus(){
    static unsigned long lastChecked = millis();
    static bool wasUpdated = false;
    if(wasUpdated){
        return;
    }
    //Makes sure it is connected to firebase
    if(millis() - lastChecked > 500){
        lastChecked = millis();
        if(authUID == "" || wasUpdated){
            return;
        }
        String connectionsPath = "/connections/ps4Connected";
        json.set(connectionsPath.c_str(), true);
        Serial.printf("Set json... %s\n", Firebase.RTDB.updateNode(&stream, pathToData.c_str(), &json) ? "ok" : stream.errorReason().c_str());
        wasUpdated = true;
    }
}

void FirebaseManager::controllerUpdateCarData(){
    static unsigned long lastChecked = millis();
    if(millis() - lastChecked > 100){
        lastChecked = millis();

        if(authUID == ""){
            return;
        }

        if(carState != carStateManager->getCarState()){
            //Car State
            json.set(carModePath.c_str(),carStateManager->getCarState());
            carState = carStateManager->getCarState();

            //Car Direction
            json.set(carDirectionPath.c_str(), "stop");
            carStateManager->updateSpeed("Stop", 0, 0);

            //forward 
            forwardSpeed = carStateManager->getSpeed("Forward");
            json.set(forwardSpeedPath.c_str(), forwardSpeed);

            //backward 
            backwardSpeed = carStateManager->getSpeed("Backward");
            json.set(backwardSpeedPath.c_str(), backwardSpeed);

            //right 
            rightSpeed = carStateManager->getSpeed("Right");
            json.set(rightSpeedPath.c_str(), rightSpeed);

            //left 
            leftSpeed = carStateManager->getSpeed("Left");
            json.set(leftSpeedPath.c_str(), leftSpeed);
            
            if(!Firebase.RTDB.updateNode(&stream, pathToData.c_str(), &json)){
               Serial.println(stream.errorReason().c_str());
            }

            return;
        }   
        
        if(carState == MODE_REMOTE_CONTROL){
            //Car State
            json.set(carModePath.c_str(),carStateManager->getCarState());
            carState = carStateManager->getCarState();

            //Car Direction
            directionOfCar = carStateManager->getDirection();
            json.set(carDirectionPath.c_str(), directionOfCar);

            //forward 
            forwardSpeed = carStateManager->getSpeed("Forward");
            json.set(forwardSpeedPath.c_str(), forwardSpeed);

            //backward 
            backwardSpeed = carStateManager->getSpeed("Backward");
            json.set(backwardSpeedPath.c_str(), backwardSpeed);

            //right 
            rightSpeed = carStateManager->getSpeed("Right");
            json.set(rightSpeedPath.c_str(), rightSpeed);

            //left 
            leftSpeed = carStateManager->getSpeed("Left");
            json.set(leftSpeedPath.c_str(), leftSpeed);
            
            if(!Firebase.RTDB.updateNode(&stream, pathToData.c_str(), &json)){
                Serial.println(stream.errorReason().c_str());
            }
            
            return;
        }    

    }
}

void FirebaseManager::listenStream(){
    static unsigned long lastChecked = millis();
    if(millis() - lastChecked > 100){
        lastChecked = millis();

        String currentPath = pathToData + carModePath;
        if(Firebase.RTDB.getInt(&stream, currentPath.c_str())){
            if (stream.dataType() == "int") {
                carState = stream.intData();
                carStateManager->updateCarMode(carState);
            }
        } else {
            Serial.println(stream.errorReason());
        }
        
        
        // camera esp32
        if(deviceType == DEVICE_TYPE_CAMERA){
            
            //only fetch data when in controller or web mode so not to overide the other functions
            if(carState == MODE_LINE_FOLLOW || carState == MODE_SELF_DRIVING){
                return;
            } else if (carState == MODE_INPUT_DIRECTIONS) {
                currentPath = pathToData + directionsToFollowPath;
                if (Firebase.RTDB.getString(&stream, currentPath.c_str())) {
                    if (stream.dataType() == "string") {
                        directionsToFollow = stream.stringData();
                        carStateManager->updateDirectionsToFollow(directionsToFollow);
                    }
                }
                return;
            }

            //Forward Speed
            currentPath = pathToData + forwardSpeedPath;
            if(Firebase.RTDB.getInt(&stream, currentPath.c_str())){
                if (stream.dataType() == "int") {
                    forwardSpeed = stream.intData();
                }
            } else {
                Serial.println(stream.errorReason());
            }

            //Backward Speed
            currentPath = pathToData + backwardSpeed;
            if(Firebase.RTDB.getInt(&stream, currentPath.c_str())){
                if (stream.dataType() == "int") {
                    backwardSpeed = stream.intData();
                }
            } else {
                Serial.println(stream.errorReason());
            }

            //Right Speed
            currentPath = pathToData + rightSpeedPath;
            if(Firebase.RTDB.getInt(&stream, currentPath.c_str())){
                if (stream.dataType() == "int") {
                    rightSpeed = stream.intData();
                }
            } else {
                Serial.println(stream.errorReason());
            }

            //Left Speed
            currentPath = pathToData + leftSpeedPath;
            if(Firebase.RTDB.getInt(&stream, currentPath.c_str())){
                if (stream.dataType() == "int") {
                    leftSpeed = stream.intData();
                }
            } else {
                Serial.println(stream.errorReason());
            }

            //Car Direction
            currentPath = pathToData + carDirectionPath;
            if(Firebase.RTDB.getString(&stream, currentPath.c_str())){
                if (stream.dataType() == "string") {
                    directionOfCar = stream.stringData();
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
            } else {
                Serial.println(stream.errorReason());
            }
    

        } else {
           // since controller doesnt need anyhting else no need to fetch and waste resources
           return;
        }
    }
}

void FirebaseManager::updateIPAddress(String IP){
    static unsigned long lastChecked = millis();
    static bool wasUpdated = false;
    //Makes sure it is connected to firebase
    if(millis() - lastChecked > 500){
        lastChecked = millis();
        if(authUID == "" || wasUpdated){
            return;
        }
    
        json.set(IPAddressPath.c_str(),IP);
        if(!Firebase.RTDB.updateNode(&stream, pathToData.c_str(), &json)){
            Serial.println(stream.errorReason().c_str());
        } else {
            wasUpdated = true;
        }
       
    }
}
 