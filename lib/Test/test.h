#ifndef __TEST_H__
#define __TEST_H__


#include <Arduino.h>
#include "PinDef.h"
#include "DriveManager.h"
#include "IRTrackingManager.h"

#define MAX_DIRECTIONS 16

// helper function
static boolean is_in_range(long x, long s, long e) //f(x)
{
  if (s <= x && x <= e)
    return true;
  else
    return false;
}

class Test
{
private:
    DriveManager *driver;
    IRTrackingManager *tracker;
    bool BlindDetection = false;
    bool timestamp = false;
    unsigned long motorRLTime = 0;
    unsigned long turnTimer = 0;
    String directions = "102";
    int current_turn = 0;
    int numDirections = 3;

public:
    Test(DriveManager *driver, IRTrackingManager *tracking);
    void Init();
    void getIRSensorData();
    void testDrive();
    void turnDirection(int);
    void testDriveDirections();
    void updateDirections(String);
    void printData();

private:
  boolean TrackingDetectionStatus_R = false;
  boolean TrackingDetectionStatus_M = false;
  boolean TrackingDetectionStatus_L = false;
  boolean CarIsAirborne = false;
  //int start_time = 0; // keep track of time in Tracking Mode
  volatile int TrackingData_L = 0;         //Line Tracking Module Value (Left)
  volatile int TrackingData_M = 0;         //Line Tracking Module Value (Middle)
  volatile int TrackingData_R = 0;         //Line Tracking Module Value (Right)

public:
  // used for IR sensor bounds checking
  uint16_t TrackingDetection_S = 720;
  uint16_t TrackingDetection_E = 1100;
};
#endif