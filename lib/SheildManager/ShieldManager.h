#ifndef __SHIELDMANAGER_H__
#define __SHIELDMANAGER_H__


#include <Arduino.h>
#include "PinDef.h"
#include "CarStateManager.h"

class ShieldManager
{
private:
    CarStateManager *carStateManager;
public:
    ShieldManager(CarStateManager *carStateManager);
    int getCarState();
    void detectPress();
    void externalCarStateChange(int);
};

#endif