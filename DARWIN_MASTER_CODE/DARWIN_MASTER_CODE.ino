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
#include <math.h>                               // Math library required for trig functions in compass
#include <EEPROM.h>                             // EEprom library required save/load data from arduinos long term memory
#define BNO055_SAMPLERATE_DELAY_MS 100        // Instructing the sensor to sample every 100 ms
Adafruit_BNO055 DarwinIMU = Adafruit_BNO055(55, 0x28);  // Using adafruit library to create IMU object named "DarwinIMU", giving it the id 55 and address 0x28
float thetaM;           // Pitch from Accelerometer
float phiM;             // Roll from Accellerometer
float thetaFold = 0;    // Overall System Pitch Filtered (Old)
float thetaFnew;        // Overall System Pitch Filtered (New)
float phiFold = 0;      // Overall System Roll Filtered  (Old)
float phiFnew;          // Overall System Roll Filtered (New)
float theta;            // Overall System Pitch
float phi;              // Overall System Roll
float thetaRad;         // Pitch in radians
float phiRad;           // Roll in radians 
float Xm;               // Value coming off the x magnitometer
float Ym;               // Value coming off the Y Magnitometer
float psi;              // Heading angle
float dt;
unsigned long millisOld;


//********************TOF Sensor*******************TOF Sensor********************TOF Sensor********************
#include <Adafruit_VL53L0X.h>                       // VL53L0X TOF Sensor Library
Adafruit_VL53L0X DarwinTOF = Adafruit_VL53L0X();    // Creating a new Adafruit_VL53L0X object named "DarwinTOF"
float distance;
// SCL - gray SDA - white


//********************FRONT SERVO*******************FRONT SERVO********************FRONT SERVO********************
#include <Servo.h>              // Including the servo library 
Servo TOFServo;                 // Creating a new servo object named TOFServo
#define TOFServoPin 8           // This is the pin that the signal wire is connect to the arduino through. Can be any digital out pin.
int TOFServoPos = 90;           // Creating int named TOF_Y_Pos and setting it to 90. This will be the starting position in degrees our servo turns to, and hold the value of any position we want to set the servo to later.


//********************STEPPER MOTORS*******************STEPPER MOTORS********************STEPPER MOTORS********************
#include <Stepper.h>                                // Stepper motor library (Included with Arduino IDE)
#define steps 64                                    // Setting the number of steps per rotation the motor takes. This is mechanically defined in the motors spec sheet
#define degree 5.625                                // Setting the number of degrees the motor rotates per step
int StepperSpeed = 200;                             // Speed of motor
Stepper TOFStepper(steps, A8, A10, A9, A11);          // Creating Stepper object named TOFStepper and defining the steppers pins


//--------------VOID SETUP ------------ VOID SETUP ------------ VOID SETUP ---------------- VOID SETUP --------------


void setup() {

  //Bluetooth Serial Communication
  Serial.begin(9600);                                                // Begin bluetooth communication at 9600
  Serial.println("Bluetooth Serial Communication Established\n");    // Confirm bluetooth connection established
  // Pin TX2 and RX2 -- Yellow and Blue

  //TOF Sensor Setup
  Serial.println("TOF Setup Begin");        // Printing for debugging
  DarwinTOF.begin();                        // Establish connection with TOF sensor
  Serial.println("TOF Setup Complete\n");   // Printing for debugging

  //TOF Stepper Setup
  TOFStepper.setSpeed(StepperSpeed);        // Setting TOF Stepper Motor speed
  
  //Servo (TOF) Setup
  Serial.println("Servo Setup Begin");      // Printing for debugging
  TOFServo.attach(TOFServoPin);             // Attaches the servo to the signalPin of the Arduino
  Serial.print("Setting Servo to ");        // Printing for debugging
  Serial.print(TOFServoPos);                // Printing for debugging
  Serial.println(" degrees");               // Printing for debugging
  TOFServo.write(TOFServoPos);              // Set servo to start position (90 degrees)sdf
  Serial.println("Servo Setup Complete");   // Printing for debugging
  Serial.println(" ");                      // Printing for debugging


  // Motor Setup                        //Setting all pins to output
  pinMode(DCmotorFrontPWMA, OUTPUT);
  pinMode(DCmotorFrontAI1, OUTPUT);
  pinMode(DCmotorFrontAI2, OUTPUT);
  pinMode(DCmotorFrontPWMB, OUTPUT);
  pinMode(DCmotorFrontBI1, OUTPUT);
  pinMode(DCmotorFrontBI2, OUTPUT);

  pinMode(DCmotorMiddlePWMA, OUTPUT);
  pinMode(DCmotorMiddleAI1, OUTPUT);
  pinMode(DCmotorMiddleAI2, OUTPUT);
  pinMode(DCmotorMiddlePWMB, OUTPUT);
  pinMode(DCmotorMiddleBI1, OUTPUT);
  pinMode(DCmotorMiddleBI2, OUTPUT);

  pinMode(DCmotorBackPWMA, OUTPUT);
  pinMode(DCmotorBackAI1, OUTPUT);
  pinMode(DCmotorBackAI2, OUTPUT);
  pinMode(DCmotorBackPWMB, OUTPUT);
  pinMode(DCmotorBackBI1, OUTPUT);
  pinMode(DCmotorBackBI2, OUTPUT);


  //LED Tail Lights Setup
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(Led5, OUTPUT);
  pinMode(Led6, OUTPUT);
  StartUpLights();


  //IMU Sensor Setup
  Serial.println("IMU Setup Begin");
  DarwinIMU.begin();                      // Starting the IMU waiting 1 second to give it time to power on and make its connection before sending another command
  delay(1000);                            // waiting 1 second to give it time to power on and make its connection before sending another command
  int eeAddress = 0;
  long bnoID;
  bool foundCalib = false;

  EEPROM.get(eeAddress, bnoID);

  adafruit_bno055_offsets_t calibrationData;
  sensor_t sensor;
  
  DarwinIMU.getSensor(&sensor);

  if (bnoID != sensor.sensor_id)
    {
        Serial.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    }
    else
    {
        Serial.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);


        Serial.println("\n\nRestoring Calibration data to the BNO055...");
        DarwinIMU.setSensorOffsets(calibrationData);

        Serial.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }
  
  DarwinIMU.setExtCrystalUse(true);       // Dont use the crystal on the chip itself, use crystal on the board (for time keeping)
  sensors_event_t event;
  DarwinIMU.getEvent(&event);
 
  Serial.println("Move sensor slightly to calibrate magnetometers");
    while (!DarwinIMU.isFullyCalibrated())
        {
            DarwinIMU.getEvent(&event);
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }


  Serial.println("\nFully calibrated!");
  Serial.println("--------------------------------");
  Serial.println("Calibration Results: ");
  adafruit_bno055_offsets_t newCalib;
  DarwinIMU.getSensorOffsets(newCalib);
    
    
  millisOld = millis();                         // Grabbing the system time for dt variable
  Serial.println("IMU Calibration Complete");   // Printing for debugging

  // End of Void Setup
  Serial.println("End of Void Setup");          // Printing for debugging
  Serial.println(" ");                          // Printing for debugging
}





//----------VOID LOOP--------------VOID LOOP------------------VOID LOOP-----------------VOID LOOP---------------


void loop() {

//TurnLeft(90);
LightsOut();
IMUDirection();
GetDistance();
}
