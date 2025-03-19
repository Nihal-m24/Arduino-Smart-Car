
#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <Arduino.h>
#include "IRTrackingManager.h"


class ModeManager
{
public:
  void Init(void);
  void Bootup(void);
//   void RGB(void);
//   void Expression(void);
//   void Rocker(void);             //APP Rocker Control
  void Tracking(void);           //Line Tracking Mode
  void Obstacle(void);           //Obstacle Avoidance
  void Follow(void);             //Following Mode
//   void Servo(uint8_t Set_Servo); //Servo Control
  void Standby(void);            //Standby Mode
//   void KeyCommand(void);         //Mode Switch Button
  void SensorDataUpdate(void);   //Sensor Data Update

public: /*CMD*/
//   void CMD_UltrasoundModuleStatus_xxx0(uint8_t is_get);
  void CMD_TraceModuleStatus_xxx0(uint8_t is_get);
  void CMD_Car_LeaveTheGround_xxx0(uint8_t is_get);


private:
  /*Sensor Raw Value*/
  volatile float VoltageData_V;        //Battery Voltage Value
//   volatile uint16_t UltrasoundData_mm; //Ultrasonic Sensor Value (mm)
//   volatile uint16_t UltrasoundData_cm; //Ultrasonic Sensor Value (cm)
  volatile int TrackingData_L;         //Line Tracking Module Value (Left)
  volatile int TrackingData_M;         //Line Tracking Module Value (Middle)
  volatile int TrackingData_R;         //Line Tracking Module Value (Right)
  /*Sensor Status*/
  boolean VoltageDetectionStatus = false;
  boolean UltrasoundDetectionStatus = false;
  boolean TrackingDetectionStatus_R = false;
  boolean TrackingDetectionStatus_M = false;
  boolean TrackingDetectionStatus_L = false;

public:
  boolean Car_LeaveTheGround = true;

  /*Sensor Threshold Setting*/
  const float VoltageDetection = 7.00;
  const uint8_t ObstacleDetection = 20;


public:
  uint8_t TrackingDetection_S = 250;
  uint16_t TrackingDetection_E = 850;
  uint16_t TrackingDetection_V = 950;


};
extern ModeManager manager;
#endif
