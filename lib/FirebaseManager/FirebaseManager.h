#ifndef FIREBASEMANAGER_H
#define FIREBASEMANAGER_H

#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <HTTPClient.h>
#include "PinDef.h"
#include "FirebaseKeys.h"
#include "CarStateManager_ESP.h"
#include <map>

class FirebaseManager
{
private:
    FirebaseData stream;
    FirebaseAuth auth;
    FirebaseConfig config;
    FirebaseJson json;

    String pathToData;
    String authUID;
    CarStateManagerESP *carStateManager;

    int forwardSpeed;
    int backwardSpeed;
    int rightSpeed;
    int leftSpeed;
    String directionOfCar; 
    String directionsToFollow;
    int carState;
    long lastTime;

    String forwardSpeedPath = "/carData/forwardSpeed";
    String backwardSpeedPath = "/carData/backwardSpeed";
    String rightSpeedPath = "/carData/rightSpeed";
    String leftSpeedPath = "/carData/leftSpeed";
    String carModePath = "/carData/carMode";
    String carDirectionPath = "/carData/carDirection";
    String lastUpdatePath = "/carData/lastUpdate";
    String IPAddressPath = "/IPAddress/IP";
    String directionsToFollowPath = "/carData/directionsToFollow";

    int deviceType;
    
public:
    FirebaseManager(CarStateManagerESP *carStateManager, int type); 
    void getUID();
    void listenStream();
    void updateControllerStatus();
    void controllerUpdateCarData();
    void updateIPAddress(String IP);
};

#endif