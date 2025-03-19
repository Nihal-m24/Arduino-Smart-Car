#include "PS4ControlManager.h"

PS4ControlManager::PS4ControlManager(CarStateManagerESP *carStateManager){
    PS4.begin("AC:49:DB:79:F4:60");
    this->carStateManager = carStateManager;
    static int lastReset = 0;

    //Removes connected devices after every 5 connections or else causes memeory issues
    if(lastReset == 5){
        uint8_t pairedDeviceBtAddr[20][6];  
        int count = esp_bt_gap_get_bond_device_num();
        esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
        for(int i = 0; i < count; i++) 
        {
          esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
        }
        lastReset = 0;
    } else {
        lastReset++;
    }

    updatePS4LED();
}

long PS4ControlManager::map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void PS4ControlManager::checkRightJoyStick(){
    int8_t rightJYSTCK_X = 0;
    int8_t rightJYSTCK_Y = 0;
    long xSpeed = 0;
    long ySpeed = 0;
    String carDirection = "";

    if(PS4.isConnected()){
        //IDK What to do with the value yet
        rightJYSTCK_X = PS4.RStickX();
        rightJYSTCK_Y = PS4.RStickY();

        //Check if going forward, backward, fwright, fwleft, bwright, bwleft, right or left than calculate speed based on throtle 
        //Checking if going forward or backward and checking the speed
        if(rightJYSTCK_Y <= -20 || rightJYSTCK_Y >= 20){
            updateCarMode(MODE_REMOTE_CONTROL);
            if(rightJYSTCK_Y < 0){
                //Going backward
                ySpeed =  map(static_cast<long>(-1*rightJYSTCK_Y), 20, 127, 50, 200);
                carDirection = "Backward";
            } else {
                //Going forward
                ySpeed =  map(static_cast<long>(rightJYSTCK_Y), 20, 127, 50, 200);
                carDirection = "Forward";
            }
        }

        if(rightJYSTCK_X <= -20 || rightJYSTCK_X >= 20){
            updateCarMode(MODE_REMOTE_CONTROL);
            if(rightJYSTCK_X < 0){
                //Going Left
                xSpeed =  map(static_cast<long>(-1*rightJYSTCK_X), 20, 127, 50, 200);
                carDirection += "Left";
            } else {
                //Going Right
                xSpeed =  map(static_cast<long>(rightJYSTCK_X), 20, 127, 50, 200);
                carDirection += "Right";
            }
        }

        //Update the speed of the cars
        carStateManager->updateSpeed(carDirection, ySpeed, xSpeed);
        /*
        Serial.print("Car is moving ");
        Serial.print(carStateManager->getDirection());
        Serial.print(" with speed ");
        Serial.print("F: ");
        Serial.print(carStateManager->getSpeed("Forward"));
        Serial.print(" B: ");
        Serial.print(carStateManager->getSpeed("Backward"));
        Serial.print(" R: ");
        Serial.print(carStateManager->getSpeed("Right"));
        Serial.print(" L: ");
        Serial.print(carStateManager->getSpeed("Left"));
        Serial.println();
         */
    }
}

void PS4ControlManager::updateCarMode(int mode){
    //add contorler
    carStateManager->updateCarMode(mode);
    updatePS4LED();
}

void PS4ControlManager::checkModePress(){
    if(PS4.isConnected()){
        if (PS4.Cross()) updateCarMode(MODE_REMOTE_CONTROL);
        if (PS4.Square()) updateCarMode(MODE_LINE_FOLLOW);
        if (PS4.Triangle()) updateCarMode(MODE_SELF_DRIVING);
        if (PS4.Circle()) updateCarMode(MODE_INPUT_DIRECTIONS);
    }
}

void PS4ControlManager::updatePS4LED(){
    static int lastMode = 0;
    int carMode = carStateManager->getCarState();
    if(lastMode != carMode){
        if(PS4.isConnected()){
            switch (carMode){
                case MODE_REMOTE_CONTROL:
                  PS4.setLed(255, 127, 0); //Orange
                  break;
                case MODE_LINE_FOLLOW:
                  PS4.setLed(0, 255, 255); //Cyan
                  break;
                case MODE_SELF_DRIVING:
                  PS4.setLed(255, 0, 255); //Purple
                  break;
                case MODE_INPUT_DIRECTIONS:
                  PS4.setLed(255, 227, 3); //Yellow
                  break;
                default:
                  PS4.setLed(0, 255, 0);
                  break;
            }
            lastMode = carMode;
            PS4.sendToController();
        }
    }
}

