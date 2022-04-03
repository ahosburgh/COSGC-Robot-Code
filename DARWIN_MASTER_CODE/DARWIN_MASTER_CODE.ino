 /* 2021-2022 NASA COSGC FRCC
  Project DARWIN
  Team Members
  Adam Hosburgh
  Nick Woolsey

*/
//--------------------SETUP--------------------SETUP--------------------SETUP--------------------SETUP--------------------


//********************DC Motors*******************DC Motors********************DC Motors********************
//DC motors only require that we define their pins. No libraries or special objects required. A = Left, B = Right, PWM = power(0-255)
int MovementDelay = 1000;
int slow = 80; // Speed of slow motors
int med = 150;
int fast = 255;
#define DCmotorFrontPWMA 6    // Front Right
#define DCmotorFrontAI1 50
#define DCmotorFrontAI2 48

#define DCmotorFrontPWMB 7    // Front Left
#define DCmotorFrontBI1 51
#define DCmotorFrontBI2 49

#define DCmotorMiddlePWMA 3   // Middle Right
#define DCmotorMiddleAI1 44
#define DCmotorMiddleAI2 42

#define DCmotorMiddlePWMB 4   // Middle Left
#define DCmotorMiddleBI1 45
#define DCmotorMiddleBI2 43

#define DCmotorBackPWMA 10    // Back Right
#define DCmotorBackAI1 38
#define DCmotorBackAI2 36

#define DCmotorBackPWMB 11    // Back LEft
#define DCmotorBackBI1 39
#define DCmotorBackBI2 37


//********************LED Pins*******************LED Pins********************LED Pins********************
// Defining LED pins for the taillights
#define Led1 A0
#define Led2 A1
#define Led3 A2
#define Led4 A5
#define Led5 A4
#define Led6 A3
const unsigned long eventInterval = 2000;
unsigned long previousTime = 0;

//********************IMU Sensor*******************IMU Sensor********************IMU Sensor********************
#include <Wire.h>                               // I2C comunication library (built into Arduino)
#include <Adafruit_Sensor.h>                    // Adafruit Unified Sensor Library
#include <Adafruit_BNO055.h>                    // Adafruit BNO055 Library 
#include <utility/imumaths.h>                   // Utility included in one of the 2 libraries above, but needs to be called specifically
#include <math.h>                               // Math library required for trig functions in compas
#define BNO055_SAMPLERATE_DELAY_MS 100          // Instructing the sensor to sample every 100 ms
Adafruit_BNO055 DarwinIMU = Adafruit_BNO055(55, 0x28);  // Using adafruit library to create IMU object named "DarwinIMU", giving it the id 55 and address 0x28

unsigned long millisOld = 0;// Old value of time in milliseconds
int GoldenDirection = 0;  // The direction if the final goal

float pi = 3.14159265;

//********************TOF Sensor*******************TOF Sensor********************TOF Sensor********************
#include <Adafruit_VL53L0X.h>                       // VL53L0X TOF Sensor Library
Adafruit_VL53L0X DarwinTOF = Adafruit_VL53L0X();    // Creating a new Adafruit_VL53L0X object named "DarwinTOF"
int objectWidth[10];                             // width of objects found
int objectHeight[10];                              // height of objects 
int objectAngle[10];
int arrayCounter = 0;
// SCL - gray SDA - white


//********************FRONT SERVO*******************FRONT SERVO********************FRONT SERVO********************
#include <Servo.h>              // Including the servo library 
Servo TOFServo;                 // Creating a new servo object named TOFServo
#define TOFServoPin 8           // This is the pin that the signal wire is connect to the arduino through. Can be any digital out pin.
int TOFServoPos = 90;           // Creating int named TOF_Y_Pos and setting it to 90. This will be the starting position in degrees our servo turns to, and hold the value of any position we want to set the servo to later.
// RANGE 0 - 170 for safety

//********************STEPPER MOTORS*******************STEPPER MOTORS********************STEPPER MOTORS********************
#include <Stepper.h>                                // Stepper motor library (Included with Arduino IDE)
#define steps 64                                    // Setting the number of steps per rotation the motor takes. This is mechanically defined in the motors spec sheet
#define degree 5.625                                // Setting the number of degrees the motor rotates per step
int StepperSpeed = 600;                             // Speed of motor
bool LorR = false;  // false for Left, True for right
int stepperAngle = 0;                               // A global counter for determining the angle of the stepper motor 
int centerOffset = 15;
int fastCenter = 0;                                 
Stepper TOFStepper(steps, A8, A10, A9, A11);        // Creating Stepper object named TOFStepper and defining the steppers pins
