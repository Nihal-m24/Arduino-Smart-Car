#include "ShieldManager.h"

ShieldManager::ShieldManager(CarStateManager *carStateManager){
    this->carStateManager = carStateManager;
    pinMode(ARDUINO_SHIELD_BUTTON_PIN, INPUT);
}

void ShieldManager::detectPress(){
    static int carState = 0;
    static bool canUpdate = true;
    int state = digitalRead(ARDUINO_SHIELD_BUTTON_PIN);
    if(canUpdate && state == 0){
        //Button pressed
        canUpdate = false;
        carState = carStateManager->getCarState();
        carState += 1;
        if(carState > 4){
            carState = 0;
        }
        carStateManager->changeCarState(carState);
    } else if (state == 1){
        //button not pressed
        canUpdate = true;
    } 
}
