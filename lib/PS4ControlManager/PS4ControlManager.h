#ifndef PS4CONTROLMANAGER_H
#define PS4CONTROLMANAGER_H

#include <Arduino.h>
#include "PinDef.h"
#include <PS4Controller.h>
#include "CarStateManager_ESP.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"

class PS4ControlManager
{
private:
    long map(long x, long in_min, long in_max, long out_min, long out_max);
    void updateCarMode(int mode);
    CarStateManagerESP *carStateManager;
public:
    PS4ControlManager(CarStateManagerESP *carStateManager);
    void checkRightJoyStick();
    void checkModePress();
    void updatePS4LED();
};


#endif