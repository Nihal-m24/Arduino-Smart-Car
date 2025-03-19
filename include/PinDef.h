#include <Arduino.h>
//Servo Motor
#define PIN_Servo_Z 10 //Confirmed. We use P10 connector. 



// Speed of Car
#define MOTOR_MAX_SPEED 250
#define MOTOR_MIN_SPEED 50

//The Motor Pins Connected To Arduino Uno
#define PIN_Motor_Speed_Right 5 //"Speed" of LEFT motors
#define PIN_Motor_Speed_Left 6 //"Speed" of RIGHT motors
#define PIN_Motor_Power_Right 7 //Power to LEFT motors 
#define PIN_Motor_Power_Left 8 //Power to RIGHT motors
#define PIN_Motor_STBY 3 //standby pin - must be HIGH to work

//RGB LED On Top Of Car
#define PIN_RBGLED 4
#define NUM_LEDS 1

//Ultrasonic Sensor
#define TRIG_PIN 13      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 12      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//Shield Button
#define ARDUINO_SHIELD_BUTTON_PIN 2

#define DEVICE_ID "CA-216"

//Operation Modes
#define MODE_REMOTE_CONTROL 1 //Can be driven from webapge or PS4 remote
#define MODE_LINE_FOLLOW 2
#define MODE_SELF_DRIVING 3
#define MODE_INPUT_DIRECTIONS 4
#define MODE_RECIEVE_TEST_RED 5

//Car Volatge
#define VOL_MEASURE_PIN A3

//esp32
#define RXD2 3
#define TXD2 40

//wifi
#define WIFI_SSID "WIFI_NAME"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define WIFI_SSID_HOTSPOT "WIFI_NAME"
#define WIFI_PASSWORD_HOTSPOT "WIFI_PASSWORD"

#define WIFI_SSID_HOME_B "WIFI_NAME"
#define WIFI_PASSWORD_B "WIFI_PASSWORD"

#define WIFI_TYPE_HOME 0
#define WIFI_TYPE_HOTSPOT 1
#define WIFI_TYPE_HOME_B 2

#define DEVICE_TYPE_CONTROLLER 0
#define DEVICE_TYPE_CAMERA 1