#ifndef REMOTE_DRIVE_MANAGER_H
#define REMOTE_DRIVE_MANAGER_H

#include <Arduino.h>
#include <CarStateManager.h>
#include <DriveManager.h>

class RemoteDriveManager
{
private:
    CarStateManager *carStateManager;
    DriveManager *driveManager;
public:
    RemoteDriveManager(CarStateManager *carStateManager, DriveManager *driveManager);
    void driveCar();
};



#endif