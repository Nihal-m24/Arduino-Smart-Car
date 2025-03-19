#ifndef __LEDMANAGER_H__
#define __LEDMANAGER_H__

#include <Arduino.h>
#include <FastLED.h>
#include "PinDef.h"

class LEDManager{
private:
    CRGB leds[NUM_LEDS];
public:
    LEDManager();
    void setLEDColor(int, int, int);
};

#endif 