#ifndef __CARSTATEMANAGER_H__
#define __CARSTATEMANAGER_H__


#include <Arduino.h>
#include "PinDef.h"
#include "DriveManager.h"

class CarStateManager
{
private:
    int forwardSpeed;
    int backwardSpeed;
    int rightSpeed;
    int leftSpeed;
    String directionOfCar; 
    String directionsToFollow;
    int carState;
    DriveManager *driveManager;
public:
    CarStateManager(DriveManager *driveManager);
    int getCarState();
    void changeCarState(int);
    void updateSpeed(String direction, int speedY, int speedX);
    void updateDirectionsToFollow(String newDirections);
    String getDirection();
    int getSpeed(String direction);
    String getDirectionsToFollow();
};

#endif