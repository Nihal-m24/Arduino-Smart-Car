#include "test.h"


Test::Test(DriveManager *driver, IRTrackingManager *tracker) {
    this->driver = driver;
    this->tracker = tracker;
}

void Test::Init() {
  if (timestamp == false) {
    BlindDetection = true;
    timestamp = true;
    motorRLTime = 0;
  }
}

void Test::getIRSensorData() {
  { /*value updation for the IR sensors on the line tracking module：for the line tracking mode*/
    TrackingData_R = tracker->getAnalogue_R();
    TrackingDetectionStatus_R = is_in_range(TrackingData_R, TrackingDetection_S, TrackingDetection_E);
    TrackingData_M = tracker->getAnalogue_M();
    TrackingDetectionStatus_M = is_in_range(TrackingData_M, TrackingDetection_S, TrackingDetection_E);
    TrackingData_L = tracker->getAnalogue_L();
    TrackingDetectionStatus_L = is_in_range(TrackingData_L, TrackingDetection_S, TrackingDetection_E);
    CarIsAirborne = !tracker->isOnGround();
  }
}

void Test::printData() {
  Serial.print("Left sensor value: ");
  // Serial.println(TrackingDetectionStatus_L);
  Serial.println(TrackingData_L);
  Serial.print("Middle sensor value: ");
  // Serial.println(TrackingDetectionStatus_M);
  Serial.println(TrackingData_M);
  Serial.print("Right sensor value: ");
  // Serial.println(TrackingDetectionStatus_R);
  Serial.println(TrackingData_R);
  Serial.print("CarisAirborne: ");
  Serial.println(CarIsAirborne);
}


void Test::testDrive() {
  static boolean timestamp = this->timestamp;
  static boolean BlindDetection = this->BlindDetection;
  static unsigned long MotorRL_time = this->motorRLTime;
  static unsigned long turnTime = this->turnTimer;
  static bool detect_fork = false;
  // const unsigned long center_car_interval = 20;
  const unsigned long direction_cooldown_interval = 500; // time to turn

  // Serial.print("Directions: ");
  // Serial.print(this->numDirections);
  // Serial.print(", ");
  // Serial.println(this->directions);
  // Serial.print("Current turn: ");
  // Serial.println(this->current_turn);

  if (detect_fork) {
    // Serial.print("timer: ");
    // Serial.println(millis() - turnTime);
    // if (millis() - turnTime >= center_car_interval) {
      if (millis() - turnTime >= direction_cooldown_interval) {
        detect_fork = false;
        turnTime = 0;
        if (this->current_turn >= this->numDirections) {
          this->current_turn = 0;
        } else {
          this->current_turn++;
        }
      } else {
        this->turnDirection(current_turn);
      }
    // } else {
    //   driver->forward(90);
    // }
  }
  else if (TrackingDetectionStatus_M && 
            TrackingDetectionStatus_L && 
            TrackingDetectionStatus_R)
  {
    /* Detect fork */
    turnTime = millis();
    detect_fork = true;
  }
  else if (TrackingDetectionStatus_M)
  {
    /*Achieve straight and uniform speed movement*/
    driver->forward(90);
    timestamp = true;
    BlindDetection = true;
  } 
  else if (TrackingDetectionStatus_R)
  {
    /*Turn right*/
    driver->right(90);
    timestamp = true;
    BlindDetection = true;
  }
  else if (TrackingDetectionStatus_L)
  {
  /*Turn left*/
    driver->left(90);
    timestamp = true;
    BlindDetection = true;
  }
  else //The car is not on the black line. execute Blind scan
  {
    //driver->stop();
    if (timestamp == true) //acquire timestamp
    {
        timestamp = false;
        MotorRL_time = millis();
        driver->stop();
    }
    /*Blind Detection*/
    if ((is_in_range((millis() - MotorRL_time), 0, 200) || 
          is_in_range((millis() - MotorRL_time), 1600, 2000)) && 
          BlindDetection == true)
    {
        driver->right(80);
    }
    else if (((is_in_range((millis() - MotorRL_time), 200, 1600))) && BlindDetection == true)
    {
        driver->left(80);
    }
    else if ((is_in_range((millis() - MotorRL_time), 3000, 3500))) // Blind Detection ...s ?
    {
        BlindDetection = false;
        driver->stop();
    }
  }
}

/* detect fork and turn at fork

*/
void Test::turnDirection(int current_turn) {
  if (current_turn >= numDirections) {
    return;
  }
  if (directions.charAt(current_turn) == '0') {
    driver->left(90);
  } else if (directions.charAt(current_turn) == '1') {
    driver->right(90);
  } else if (directions.charAt(current_turn) == '2') {
    driver->forward(90);
  }
}

void Test::updateDirections(String newDirections) {
  static int updateTimer = 0;
  const int updateInterval = 500;
  int n = newDirections.length();
  if (n < MAX_DIRECTIONS && millis() - updateTimer >= updateInterval) {
    // Serial.print("Directions: ");
    // Serial.println(newDirections);
    updateTimer = millis();
    this->directions = newDirections;
    this->numDirections = n;
    this->current_turn = 0;
  }
}
