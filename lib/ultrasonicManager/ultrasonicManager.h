#ifndef __ULTRASONICMANAGER_H__
#define __ULTRASONICMANAGER_H__


#include <DriveManager.h>
#include <ServoManager.h>
#include "PinDef.h"
#include <Arduino.h>
#include <NewPing.h>
#include <MPU6050.h>

class UltrasonicManager
{


private:
    
    int trigPin;
    int echoPin;
    int maxDistance;
    bool scanning;
    bool turningComplete = false;

    const float turningShreshold = 3.0f;
    

    MPU6050 *mpu;
    NewPing *sonar;
    ServoManager *servoManager;
    DriveManager *driveManager;
    static void echoCheckStatic();
  
    //managerwise time variables
    unsigned long lastPingTime = 0; 
    unsigned long lastScanTime = 0;
    unsigned long lastSafetyCheckTime = 0;
    
    unsigned long currentTime = 0; //For UltrasonicMode()
    unsigned long lastUpdateTime = 0; //For UltrasonicMode()
    unsigned long lastImuUpdate = 0; //For turning the cart

    unsigned long currentTimeScan = 0;
    unsigned long lastUpdateTimeScan = 0;

    unsigned long lastGyroTime = 0;



    const unsigned int updateInterval = 50; 
    const unsigned long pingInterval = 50; 
    const unsigned int safetyCheckInterval = 5000; //Wait 800ms before checking if the path is clear. 
    
    float currentHeading = 0.0f;//For turning the cart
    float desiredHeading = 0.0f;//For turning the cart
    float gyroZoffset = 0.0f;    //For turning the cart

    void gyroUpdate();
    void checkTurningProgress();




public:
    UltrasonicManager();
    ~UltrasonicManager();
    
    void UltrasonicMode();
    void ping();

    
    unsigned int getDistanceCM();
    unsigned int getDistanceINCH();
    
    void updateDistance(int maxDistance);
    void servoScanSurroundings();
    void enableScan();
    volatile unsigned int lastDistance = 0;

};









#endif