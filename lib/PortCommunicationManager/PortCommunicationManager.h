#ifndef __PORTCOMMUNICATIONMANAGER_H__
#define __PORTCOMMUNICATIONMANAGER_H__


#include <Arduino.h>
#include <ArduinoJson.h>
#include "PinDef.h"
#include "CarStateManager.h"
#include "DriveManager.h"
#include "VoltageManager.h"

class PortCommunicationManager
{
private:
    CarStateManager *carStateManager;
    DriveManager *driveManager;
    VoltageManager *voltageManager;

    unsigned long lastSendTime;
    unsigned long lastReceiveTime;
    unsigned long heartBeat;

    int forwardSpeed;
    int backwardSpeed;
    int rightSpeed;
    int leftSpeed;
    String directionOfCar;
    String directionsToFollow;
    int carState;
    
    // Servo
    uint8_t servoAngle;
    uint8_t servoSpeed;
    // Radar
    uint16_t radarDistance;
    // IR captor
    uint8_t onGround;
    // Battery
    float batteryVoltage;

    boolean syncRequestReceived;
    boolean syncRequestSent;

    void processData(String serialPortData);

    String getValue(String data, String key);

public:
    PortCommunicationManager(CarStateManager *, DriveManager*, VoltageManager*);
    void recieveData();
    void sendData();
};

#endif