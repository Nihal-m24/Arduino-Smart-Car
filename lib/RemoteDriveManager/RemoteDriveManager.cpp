#include "RemoteDriveManager.h"

RemoteDriveManager::RemoteDriveManager(CarStateManager *carStateManager, DriveManager *driveManager){
    this->carStateManager = carStateManager;
    this->driveManager = driveManager;
}


void RemoteDriveManager::driveCar(){
    String currentDirection = carStateManager->getDirection();
    if(currentDirection == "Forward"){
        driveManager->forward(carStateManager->getSpeed("Forward"));
    } else if(currentDirection == "Backward"){
        driveManager->backward(carStateManager->getSpeed("Backward"));
    } else if(currentDirection == "Right"){
       driveManager->right(carStateManager->getSpeed("Right"));
    } else if(currentDirection == "Left"){
       driveManager->left(carStateManager->getSpeed("Left"));
    } else if(currentDirection == "ForwardRight"){
       driveManager->forwardRight(carStateManager->getSpeed("Forward"));
    } else if(currentDirection == "ForwardLeft"){
       driveManager->forwardLeft(carStateManager->getSpeed("Forward"));
    } else if(currentDirection == "BackwardRight"){
       driveManager->backwardRight(carStateManager->getSpeed("Backward"));
    } else if(currentDirection == "BackwardLeft"){
       driveManager->backwardLeft(carStateManager->getSpeed("Backward"));
    } else if(currentDirection == "Stop"){
       driveManager->stop();
    } else {
        driveManager->stop();
    }
 }