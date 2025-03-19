#include "IRTrackingManager.h"


IRTrackingManager::IRTrackingManager()
{
    pinMode(PIN_ITR_L, INPUT);
    pinMode(PIN_ITR_M, INPUT);
    pinMode(PIN_ITR_R, INPUT);
}

float IRTrackingManager::getAnalogue_R()
{
    return analogRead(PIN_ITR_R);
}

float IRTrackingManager::getAnalogue_L()
{
    return analogRead(PIN_ITR_L);
}

float IRTrackingManager::getAnalogue_M()
{
    return analogRead(PIN_ITR_M);
}

uint8_t IRTrackingManager::isOnGround()
{
    if (getAnalogue_L() > IR_GROUNDED_THRESHOLD || getAnalogue_M() > IR_GROUNDED_THRESHOLD || getAnalogue_R() > IR_GROUNDED_THRESHOLD)
    {
        return 0;
    }
    return 1;
}

// void IRTrackingManager::testIRTracking()
// {
//     if (!isOnGround())
//     {
//         Serial.println("Car is airborne !");
//     }
// }