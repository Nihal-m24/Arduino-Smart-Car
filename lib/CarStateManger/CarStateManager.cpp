#include "CarStateManager.h"

CarStateManager::CarStateManager(DriveManager *driveManager){
    this->driveManager = driveManager;
    carState = 0;
}

int CarStateManager::getCarState(){
    return carState;
}

void CarStateManager::changeCarState(int newState){
    if(newState > 5){
        carState = 0;
        return;
    }
    carState = newState;
    driveManager->stop(); //stops the car when the state changes
}

void CarStateManager::updateSpeed(String direction, int speedY, int speedX){
    
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

void CarStateManager::updateDirectionsToFollow(String newDirections) {
    directionsToFollow = newDirections;
}

String CarStateManager::getDirection(){
    return directionOfCar;
}

int CarStateManager::getSpeed(String direction){
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

String CarStateManager::getDirectionsToFollow() {
    return directionsToFollow;
}