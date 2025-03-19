#include "ultrasonicManager.h"

//Credit to Tim Eckel - teckel@leethost.com NewPing library. The below section are re-used from the NewPing library
//#define NewPingConvert(echoTime, conversionFactor) (max((echoTime + conversionFactor / 2) / conversionFactor, (echoTime ? 1 : 0))) //Credit to Tim Eckel - teckel@leethost.com NewPing library
//#define US_ROUNDTRIP_IN 146     // Microseconds (uS) it takes sound to travel round-trip 1 inch (2 inches total), uses integer to save compiled code space.
//#define US_ROUNDTRIP_CM 57      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
//Credit to Tim Eckel - teckel@leethost.com NewPing library. The above section are re-used from the NewPing library

#define stopDistance 35
#define TIME_CONSTANT 20

UltrasonicManager::~UltrasonicManager(){
    delete sonar;
    delete servoManager;
    delete driveManager;
    delete mpu;
}

UltrasonicManager::UltrasonicManager(){
    this->trigPin = TRIG_PIN;
    this->echoPin = ECHO_PIN;
    this->maxDistance = MAX_DISTANCE;

    this->servoManager = new ServoManager();
    this->driveManager = new DriveManager();
    this->mpu = new MPU6050();
    this->sonar = new NewPing(trigPin, echoPin, maxDistance);


    scanning = true;
    lastDistance = 999;
    
    mpu->initialize();
    mpu->CalibrateGyro(4);

}

unsigned int UltrasonicManager::getDistanceCM()
{
    return lastDistance;

}

void UltrasonicManager::ping() {

    if (millis() - lastPingTime >= pingInterval) {
        lastPingTime = millis();
        lastDistance = sonar->ping_cm(MAX_DISTANCE); // Non-blocking approach
    }
}

enum drive_STATES  {DRIVE_WAIT,DRIVE_SCAN,DRIVE_FORWARD, DRIVE_SAFETY_CHECK};
drive_STATES drive_STATE = DRIVE_WAIT;

unsigned int t = 0;

void UltrasonicManager::UltrasonicMode()
{
    currentTime = millis();
    if (currentTime - lastPingTime >= 50) {
        ping();
    }


    switch(drive_STATE)
    {
        case DRIVE_WAIT:
            if (lastDistance < stopDistance) {
                drive_STATE = DRIVE_SCAN;
                scanning = true;
                driveManager->stop();
            } else {
                drive_STATE = DRIVE_FORWARD;
            }
            break;

        case DRIVE_SCAN:
            // Let servoScanSurroundings do its thing:
            servoScanSurroundings();
            // If scanning is done, transition to safety check
            if (!scanning&&turningComplete) {
                lastSafetyCheckTime = millis();
                drive_STATE = DRIVE_SAFETY_CHECK;
            }
            break;

        case DRIVE_SAFETY_CHECK:
            // Wait 500ms to confirm path is clear
            
            if (millis() - lastSafetyCheckTime >= 500) {
                drive_STATE = DRIVE_WAIT;
            }
            break;

        case DRIVE_FORWARD:
            if (lastDistance < stopDistance) {
                scanning = true;
                driveManager->stop();
                drive_STATE = DRIVE_SCAN;
            } else {
                driveManager->forward(100);


            }
            break;
    }
}
   







void UltrasonicManager::servoScanSurroundings() {
    static unsigned int bestDistance = 0; // Best distance found so far
    static unsigned int bestAngle = 0;    // Angle corresponding to best distance
    static int currentAngle = 0;          // Current angle being scanned


    if (!turningComplete) {
        // Nonblocking turn logic
        checkTurningProgress();
        // If we’re still turning after checkTurnCompletion(), don’t do scanning
        if (!turningComplete) {
            return; // we skip scanning so the robot can finish turning
        }
    }



    if (!scanning) { 
        return; 
    }

    currentTimeScan = millis();
    
    // Update the servo command only after the stabilization period
    if (currentTimeScan - lastUpdateTimeScan >= updateInterval) {
        lastUpdateTimeScan = currentTimeScan; // Reset the timer

        // Move the servo to the new position
        servoManager->setAngle(currentAngle);


        // Trigger the ultrasonic sensor and get distance

        unsigned int distanceAtTheAngle = lastDistance;

        //Serial.print("Angle: "); Serial.print(currentAngle);
        //Serial.print(" | Distance: "); Serial.println(distanceAtTheAngle);

        // Store the best angle where the largest distance was detected
        if (distanceAtTheAngle > bestDistance) {
            bestDistance = distanceAtTheAngle;
            bestAngle = currentAngle;
        }

        // Increment the angle for the next measurement
        currentAngle += 10;

        // If we've scanned the full range, finalize scanning
        if (currentAngle == 180) {

            turningComplete = false;
            desiredHeading = fmod(currentHeading + (bestAngle - 90), 360.0f);

            // Move servo to the best angle or reset position
            
            currentAngle = 0;
            bestDistance = 0;
            bestAngle = 0;
            servoManager->setAngle(90);
            scanning = false; //It's ok that we set it to false here, the check for turning 
            //will prevent state machine changes until the turning is complete
            

        }
    }
}

void UltrasonicManager::enableScan()
{
    if(scanning == false){scanning = true;}
    
}

void UltrasonicManager::gyroUpdate()
{
   unsigned long currentGyroTime = millis();
   float deltaT = (currentGyroTime - lastGyroTime) / 1000.0f; //we do integrals 
   lastGyroTime = currentGyroTime;
    
   int16_t gz;
   gz = mpu->getRotationZ();

   float gyroZ = gz / 131.0f;
   gyroZ = gyroZ - gyroZoffset;

   float angleDifference = gyroZ * deltaT;
   currentHeading += angleDifference;

   if(currentHeading < 0.0f){currentHeading += 360.0f;}
   else if(currentHeading >= 360.0f){currentHeading -= 360.0f;}
}

void UltrasonicManager::checkTurningProgress()
{

    if(!turningComplete){
        gyroUpdate();
        float diff = desiredHeading - currentHeading;
        if(diff > 180.0f){diff -= 360.0f;}
        else if(diff < -180.0f){diff += 360.0f;} //normalization minmax

        if(fabs(diff) < turningShreshold){
            turningComplete = true;
            driveManager->stop();
        }else{
            if(diff > 0.0f){
                driveManager->left(80);
            }else{
                driveManager->right(80);
            }
        }




    }

}