#ifndef SERIALCOMMUNICATIONANAGER_H
#define SERIALCOMMUNICATIONANAGER_H

#include <Arduino.h>
#include <PinDef.h>
#include "CarStateManager_ESP.h"

class SerialCommunicationManager
{
private:
CarStateManagerESP *carStateManager;
    void processData(String data);
    int forwardSpeed;
    int backwardSpeed;
    int rightSpeed;
    int leftSpeed;
    String directionOfCar; 
    String directionsToFollow;
    int carState;
    bool syncRequestReceived;
public:
    SerialCommunicationManager(CarStateManagerESP *carStateManager);
    void receiveData();
    void sendData();
    void handleUnoSyncRequest();
};

#endif