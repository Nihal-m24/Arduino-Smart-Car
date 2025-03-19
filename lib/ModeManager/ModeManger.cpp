#include <avr/wdt.h>
#include "ModeManager.h"

// define constants
#define MAX_DIRECTIONS 3

// Initialize managers here 
ModeManager manager;
IRTrackingManager IR_tracking_manager;
// UltrasonicManager ultrasonic_manager;
// VoltageManager voltage_manager;
// DriveManager motor_manager;

//helper function: checks if x is in range of [s, e]
static boolean is_in_range(long x, long s, long e) //f(x)
{
  if (s <= x && x <= e)
    return true;
  else
    return false;
}

enum Mode
{
  Standby_mode,
  Tracking_mode
};

struct CarControl {
  Mode mode;
  //Motion motion;


  // For tracking mode with direcrections
  bool track_directions = false;
  int num_directions = 0;
};

CarControl controller;

void ModeManager::Init(void) {
  // init functions for dependent managers here
  //IR_tracking_manager.IRTracking_Init();
  //ultrasonic_manager.Ultrasonic_Init();
  //voltage_manager.Voltage_Init();
  //motor_manager.Motor_Init();


  // set initial mode
  controller.mode = Standby_mode;

}

static bool ApplicationFunctionSet_SmartRobotCarLeaveTheGround(void)
{
  if (IR_tracking_manager.getAnalogue_R() > manager.TrackingDetection_V &&
      IR_tracking_manager.getAnalogue_M() > manager.TrackingDetection_V &&
      IR_tracking_manager.getAnalogue_L() > manager.TrackingDetection_V)
  {
    manager.Car_LeaveTheGround = false;
    return false;
  }
  else
  {
    manager.Car_LeaveTheGround = true;
    return true;
  }
}

void ModeManager::SensorDataUpdate(void) {
      // motor_manager.Motor_Test();
  {/*Battery voltage status update*/
    static unsigned long VoltageData_time = 0;
    static int VoltageData_number = 1;
    if (millis() - VoltageData_time > 10) //read and update the data per 10ms
    {
      VoltageData_time = millis();
      // VoltageData_V = voltage_manager.getAnalogue();
      if (VoltageData_V < VoltageDetection)
      {
        VoltageData_number++;
        if (VoltageData_number == 500) //Continuity to judge the latest voltage value multiple 
        {
          VoltageDetectionStatus = true;
          VoltageData_number = 0;
        }
      }
      else
      {
        VoltageDetectionStatus = false;
      }
    }
  }


  { /*value updation for the IR sensors on the line tracking module：for the line tracking mode*/
    TrackingData_R = IR_tracking_manager.getAnalogue_R();
    TrackingDetectionStatus_R = is_in_range(TrackingData_R, TrackingDetection_S, TrackingDetection_E);
    TrackingData_M = IR_tracking_manager.getAnalogue_M();
    TrackingDetectionStatus_M = is_in_range(TrackingData_M, TrackingDetection_S, TrackingDetection_E);
    TrackingData_L = IR_tracking_manager.getAnalogue_L();
    TrackingDetectionStatus_L = is_in_range(TrackingData_L, TrackingDetection_S, TrackingDetection_E);
    //ITR20001 Check if the car leaves the ground
    //ApplicationFunctionSet_SmartRobotCarLeaveTheGround();
  }
}

/* Tracking Function */
void ModeManager::Tracking(void) {
    static boolean timestamp = true;
    static boolean BlindDetection = true;
    static unsigned long MotorRL_time = 0;
    static int current_turn = 0;
    static int directions[MAX_DIRECTIONS] = {0,1,1}; // 0 = Left, 1 = Right
    static int direction_cooldown_timer = 0;
    if (controller.mode == Tracking_mode)
    {
      if (Car_LeaveTheGround == false) //Check if the car leaves the ground
      {
        // Stop car if IR tracking senses car is too high
        // motor_driver.stop();
        //return;
      }
      /* if "special button" is pressed, start reading directional inputs from controller
         if "special button" is pressed again, then stop reading inputs, start following dirctions


         if (controller.getButtonKeyValue() == 4) {
           controller.track_direction = true;
         
         }
      */
      if (controller.track_directions) {

        /* start reading inputs 
        switch (controller.getButtonKeyValue)

        case 4 (right)

        case 5 (left)
        }
        
        */
        
        if (current_turn == controller.num_directions) {
          // clear directions

          // destination reached

        } else {
          // detect fork
          if (TrackingDetectionStatus_L && TrackingDetectionStatus_R) {
            if (direction_cooldown_timer > 50) {
              if (!directions[current_turn]) {
                // Go Left

                current_turn++;
                direction_cooldown_timer = 0;
              }
              else {
                // Go Right

                current_turn++;
                direction_cooldown_timer = 0;
              }
            }
            else {
              direction_cooldown_timer++;
            }
          }
        }

      }
      if (TrackingDetectionStatus_M)
      {
        /*Achieve straight and uniform speed movement*/
        // controller(Forward, 100);
        timestamp = true;
        BlindDetection = true;
      }
      else if (TrackingDetectionStatus_R)
      {
        /*Turn right*/
        // controller(Right, 100);
        timestamp = true;
        BlindDetection = true;
      }
      else if (TrackingDetectionStatus_L)
      {
      /*Turn left*/
        // controller(Left, 100);
        timestamp = true;
        BlindDetection = true;
      }
      else ////The car is not on the black line. execute Blind scan
      {
        if (timestamp == true) //acquire timestamp
        {
            timestamp = false;
            MotorRL_time = millis();
            // controller(stop, 0);
        }
        /*Blind Detection*/
        if ((is_in_range((millis() - MotorRL_time), 0, 200) || is_in_range((millis() - MotorRL_time), 1600, 2000)) && BlindDetection == true)
        {
            // controller(Right, 100);
        }
        else if (((is_in_range((millis() - MotorRL_time), 200, 1600))) && BlindDetection == true)
        {
            // controller(Left, 100);
        }
        else if ((is_in_range((millis() - MotorRL_time), 3000, 3500))) // Blind Detection ...s ?
        {
            BlindDetection = false;
            // controller(stop, 0);
        }
      }
    }
    else if (false == timestamp)
    {
        BlindDetection = true;
        timestamp = true;
        MotorRL_time = 0;
    }
}

void ModeManager::Obstacle(void) {

}

void ModeManager::Follow(void) {

}