#include "ServoManager.h"

//From official Elegoo guides regarding servo usage.

ServoManager::ServoManager(){
    this->servoPinZ = PIN_Servo_Z;
    this->pulseLn = 0;
    this->servo.attach(servoPinZ,500,2400);  //500 for 0 degree, 2400 for 180 degree

}
 
void ServoManager::setAngle(int angle){
    this->pulseLn = map(angle,0,180,500,2400);
    this->servo.writeMicroseconds(pulseLn); //
}

ServoManager::~ServoManager(){
    this->servo.detach();
}