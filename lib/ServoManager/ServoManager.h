#ifndef __SERVOMANAGER_H__
#define __SERVOMANAGER_H__

#include <Arduino.h>
#include <PinDef.h>
#include <Servo.h>

class ServoManager
{
private:
    int servoPinZ;
    int pulseLn;
    Servo servo;

public:

    ServoManager();
    ~ServoManager();
    void setAngle(int angle);
    int getAngle();
    void attachServo(int servoPin);
    void detachServo();
};

#endif