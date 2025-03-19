#include "CarStateManager_ESP.h"

CarStateManagerESP::CarStateManagerESP(){
    carMode = 0;
    forwardSpeed = 0;
    backwardSpeed = 0;
    rightSpeed = 0;
    leftSpeed = 0;
    directionOfCar = "";
}

void CarStateManagerESP::updateSpeed(String direction, int speedY, int speedX){
    
    if(direction == "Forward"){
        directionOfCar = direction;
        forwardSpeed = speedY;
        backwardSpeed = 0;
        rightSpeed = 0;
        leftSpeed = 0;
    }   else if(direction == "Backward"){
        directionOfCar = direction;
        forwardSpeed = 0;
        backwardSpeed = speedY;
        rightSpeed = 0;
        leftSpeed = 0;
    }   else if(direction == "Right"){
        directionOfCar = direction;
        forwardSpeed = 0;
        backwardSpeed = 0;
        rightSpeed = speedX;
        leftSpeed = 0;
    }   else if(direction == "Left"){
        directionOfCar = direction;
        forwardSpeed = 0;
        backwardSpeed = 0;
        rightSpeed = 0;
        leftSpeed = speedX;
    }   else if(direction == "ForwardRight"){
        directionOfCar = direction;
        forwardSpeed = speedY;
        backwardSpeed = 0;
        rightSpeed = speedX;
        leftSpeed = 0;
    }   else if(direction == "ForwardLeft"){
        directionOfCar = direction;
        forwardSpeed = speedY;
        backwardSpeed = 0;
        rightSpeed = 0;
        leftSpeed = speedX;
    }   else if(direction == "BackwardRight"){
        directionOfCar = direction;
        forwardSpeed = 0;
        backwardSpeed = speedY;
        rightSpeed = speedX;
        leftSpeed = 0;
    }   else if(direction == "BackwardLeft"){
        directionOfCar = direction;
        forwardSpeed = 0;
        backwardSpeed = speedY;
        rightSpeed = 0;
        leftSpeed = speedX;
    } else {
        directionOfCar = "Stop";
        forwardSpeed = 0;
        backwardSpeed = 0;
        rightSpeed = 0;
        leftSpeed = 0;
    }
}

void CarStateManagerESP::updateCarMode(int mode){
    carMode = mode;
}

int CarStateManagerESP::getCarState(){
    return carMode;
}

void CarStateManagerESP::updateDirectionsToFollow(String newDirections) {
    directionsToFollow = newDirections;
}

String CarStateManagerESP::getDirection(){
    return directionOfCar;
}

int CarStateManagerESP::getSpeed(String direction){
    if(direction == "Forward"){
        return forwardSpeed;
    } else if(direction == "Backward"){
        return backwardSpeed;
    } else if(direction == "Right"){
        return rightSpeed;
    } else if(direction == "Left"){
        return leftSpeed;
    }
    return 0; //incase so car doesnt drive off 
} 

String CarStateManagerESP::getDirectionsToFollow() {
    return directionsToFollow;
}