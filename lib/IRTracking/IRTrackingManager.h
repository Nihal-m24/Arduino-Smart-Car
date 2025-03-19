#ifndef IRTRACKINGMANAGER_H
#define IRTRACKINGMANAGER_H

#include <Arduino.h>

#define PIN_ITR_L A2
#define PIN_ITR_M A1
#define PIN_ITR_R A0

#define IR_GROUNDED_THRESHOLD 1000 // Max value to consider the captor on ground

class IRTrackingManager
{
public:
    IRTrackingManager();
    float getAnalogue_R();
    float getAnalogue_L();
    float getAnalogue_M();
    uint8_t isOnGround();
    void testInfraRedCaptor();

private:
};

#endif