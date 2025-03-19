#ifndef VOLTAGE_MANAGER_H
#define VOLTAGE_MANAGER_H

#include <Arduino.h>
#include "PinDef.h"


class VoltageManager
{
public:
    VoltageManager();
    float getVoltage();

private:
};

#endif