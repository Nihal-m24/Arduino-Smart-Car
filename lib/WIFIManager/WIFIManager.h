#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "PinDef.h"

class WIFIManager
{
private:
public:
    WIFIManager(int type);
    String getIPAddress();
};


#endif