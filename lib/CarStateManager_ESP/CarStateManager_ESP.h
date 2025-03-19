#ifndef CARSTATEMANAGER_H
#define CARSTATEMANAGER_H

#include <Arduino.h>
#include "pinDef.h"

class CarStateManagerESP
{
private:
    int forwardSpeed;
    int backwardSpeed;
    int rightSpeed;
    int leftSpeed;
    String directionOfCar; 
    String directionsToFollow;
    int carMode;
    
public:
    CarStateManagerESP();
    void updateSpeed(String direction, int speedY, int speedX);
    void updateCarMode(int mode);
    void updateDirectionsToFollow(String newDirections);
    int getCarState();
    String getDirection();
    String getDirectionsToFollow();
    int getSpeed(String direction);
};

#endif