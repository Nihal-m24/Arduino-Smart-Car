#ifndef __DRIVEMANAGER_H__
#define __DRIVEMANAGER_H__

#include <Arduino.h>
#include "PinDef.h"

enum MotorDirection{FORWARD,BACKWARD};

enum MotorSide{LEFT,RIGHT};

class DriveManager
{
public:
    DriveManager();
    void move(short leftSpeed, short rightSpeed);
    void stop();
    void testMotor(int speed = 50);
    void forward(unsigned char speed);
    void backward(unsigned char speed);
    void left(unsigned char speed);
    void right(unsigned char speed);
    void forwardLeft(unsigned char speed);
    void forwardRight(unsigned char speed);
    void backwardLeft(unsigned char speed);
    void backwardRight(unsigned char speed);
private:
    short m_leftSpeed, m_rightSpeed;
};


#endif