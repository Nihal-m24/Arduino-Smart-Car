#include "DriveManager.h"
#include "PinDef.h"

DriveManager::DriveManager() {
    pinMode(PIN_Motor_Speed_Right, OUTPUT);
    pinMode(PIN_Motor_Speed_Left, OUTPUT);
    pinMode(PIN_Motor_Power_Right, OUTPUT);
    pinMode(PIN_Motor_Power_Left, OUTPUT);
    pinMode(PIN_Motor_STBY, OUTPUT);

    stop(); // Ensure motors are stopped initially
}

// Generic movement function
void DriveManager::move(short leftSpeed, short rightSpeed) {
    digitalWrite(PIN_Motor_STBY, HIGH); // Enable motors

    // Determine direction based on sign of speed
    if (leftSpeed >= 0) {
        digitalWrite(PIN_Motor_Power_Left, HIGH);
    } else {
        digitalWrite(PIN_Motor_Power_Left, LOW);
        leftSpeed = -leftSpeed; // Make speed positive
    }

    if (rightSpeed >= 0) {
        digitalWrite(PIN_Motor_Power_Right, HIGH);
    } else {
        digitalWrite(PIN_Motor_Power_Right, LOW);
        rightSpeed = -rightSpeed; // Make speed positive
    }

    // Ensure speeds are within range
    leftSpeed = constrain(leftSpeed, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);

    analogWrite(PIN_Motor_Speed_Left, leftSpeed);
    analogWrite(PIN_Motor_Speed_Right, rightSpeed);
}

// Moves forward with equal speed on both wheels
void DriveManager::forward(unsigned char speed) {
    move(speed, speed);
}

// Moves backward by inverting direction
void DriveManager::backward(unsigned char speed) {
    move(-speed, -speed);
}

// Turns left by slowing/stopping left wheel
void DriveManager::left(unsigned char speed) {
    move(-speed, speed);
}

// Turns right by slowing/stopping right wheel
void DriveManager::right(unsigned char speed) {
    move(speed, -speed);
}

// Forward with slight left turn
void DriveManager::forwardLeft(unsigned char speed) {
    move(speed / 2, speed);
}

// Forward with slight right turn
void DriveManager::forwardRight(unsigned char speed) {
    move(speed, speed / 2);
}

// Backward with slight left turn
void DriveManager::backwardLeft(unsigned char speed) {
    move(-speed / 2, -speed);
}

// Backward with slight right turn
void DriveManager::backwardRight(unsigned char speed) {
    move(-speed, -speed / 2);
}

// Stops both motors
void DriveManager::stop() {
    digitalWrite(PIN_Motor_STBY, LOW); // Disable motors
    analogWrite(PIN_Motor_Speed_Left, 0);
    analogWrite(PIN_Motor_Speed_Right, 0);
}

// Simple test function to move forward and back
void DriveManager::testMotor(int speed = 50) {
    static int state = 0;
    static unsigned long testDriveMilli = millis();
    if(millis() - testDriveMilli >= 1000){
        testDriveMilli = millis();
        switch (state)
        {
        case 0:
            forward(speed);
            state++;
            break;
        case 1:
            right(speed);
            state++;
            break;

        case 2:
            left(speed);
            state++;
            break;

        case 3:
            left(speed);
            state++;
            break;  
              
        case 4:
            backward(speed);
            state++;
            break;

        case 5:
            stop();
            state = 0;
            break;

        default:
            state = 0;
            break;
        }
    }
}