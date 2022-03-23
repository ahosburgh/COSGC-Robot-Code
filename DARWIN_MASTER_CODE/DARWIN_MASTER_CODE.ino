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
//float thetaG = 0;       // Pitch from Gyro
//float phiG = 0;         // Roll from Gyro
float theta;            // Overall System Pitch
float phi;              // Overall System Roll
float thetaRad;         //
float phiRad;
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
#define TOFServoPin 8          // This is the pin that the signal wire is connect to the arduino through. Can be any digital out pin.
int TOFServoPos = 90;           // Creating int named TOF_Y_Pos and setting it to 90. This will be the starting position in degrees our servo turns to, and hold the value of any position we want to set the servo to later.


//********************STEPPER MOTORS*******************STEPPER MOTORS********************STEPPER MOTORS********************
#include <Stepper.h>                                // Stepper motor library (Included with Arduino IDE)
#define steps 64                                    // Setting the number of steps per rotation the motor takes. This is mechanically defined in the motors spec sheet
#define degree 5.625                                // Setting the number of degrees the motor rotates per step
int StepperSpeed = 200;                             // Speed of motor
Stepper TOFStepper(steps, A8, A10, A9, A11);          // Creating Stepper object named TOFStepper and defining the steppers pins


//--------------VOID SETUP ------------ VOID SETUP ------------ VOID SETUP ---------------- VOID SETUP --------------


void setup() {

  //Bluetooth Serial2 Communication
  Serial2.begin(9600);                                              // Begin bluetooth communication at 9600
  Serial2.println("Bluetooth Serial2 Communication Established");    // Confirm bluetooth connection established
  Serial2.println(" ");
  // Pin TX2 and RX2 -- Yellow and Blue

  //TOF Sensor Setup
  Serial2.println("TOF Setup Begin");      // Printing for debugging
  DarwinTOF.begin();                      // Establish connection with TOF sensor
  Serial2.println("TOF Setup Complete");   // Printing for debugging
  Serial2.println(" ");                    // Printing for debuggin


  //Servo (TOF) Setup
  Serial2.println("Servo Setup Begin");    // Printing for debugging
  TOFServo.attach(TOFServoPin);           // Attaches the servo to the signalPin of the Arduino
  Serial2.print("Setting Servo to ");      // Printing for debugging
  Serial2.print(TOFServoPos);              // Printing for debugging
  Serial2.println(" degrees");             // Printing for debugging
  TOFServo.write(TOFServoPos);            // Set servo to start position (90 degrees)sdf
  Serial2.println("Servo Setup Complete"); // Printing for debugging
  Serial2.println(" ");                    // Printing for debugging


  // Motor Setup
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
  Serial2.println("IMU Setup Begin");
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
        Serial2.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    }
    else
    {
        Serial2.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

        

        Serial2.println("\n\nRestoring Calibration data to the BNO055...");
        DarwinIMU.setSensorOffsets(calibrationData);

        Serial2.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }
  
  
  //int8_t temp = DarwinIMU.getTemp();      // int8_t is a special type of int variable type that stores values from -120 to 120. super compact. Some of the measurements from the IMU are dependant on tempature. So we need to measure the tempature of the IMU first. So we will create a new variable named temp of type int8_t and set it = to a function return of the imu library that does exactly that.*/
  
  DarwinIMU.setExtCrystalUse(true);       // Dont use the crystal on the chip itself, use crystal on the board (for time keeping)
  sensors_event_t event;
  DarwinIMU.getEvent(&event);
 
  /*Serial2.println("Move sensor slightly to calibrate magnetometers");
    while (!DarwinIMU.isFullyCalibrated())
        {
            DarwinIMU.getEvent(&event);
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }

*/
    Serial2.println("\nFully calibrated!");
    Serial2.println("--------------------------------");
    Serial2.println("Calibration Results: ");
    adafruit_bno055_offsets_t newCalib;
    DarwinIMU.getSensorOffsets(newCalib);
    
    
  millisOld = millis();                   // Grabbing the system time for dt variable
  Serial2.println("Calling IMU Calibration Function");   // Printing for debugging
  Serial2.println(" ");
  //SetupIMU();                                   // Calling function to calibrate IMU
  Serial2.println("IMU Calibration Complete");   // Printing for debugging
  Serial2.println(" ");

  // End of Void Setup
  Serial2.println("End of Void Setup");    // Printing for debugging
  Serial2.println(" ");                    // Printing for debugging

  TOFStepper.setSpeed(StepperSpeed);      // Setting TOF Stepper Motor speed
}





//----------VOID LOOP--------------VOID LOOP------------------VOID LOOP-----------------VOID LOOP---------------


void loop() {


MoveForward();
delay (3000);
DCStop();
MoveBack();
delay (1000);
DCStop();
TurnLeft(90);
TurnRight(90);

}





//==========FUNCTIONS==========FUNCTIONS==========FUNCTIONS==========FUNCTIONS==========FUNCTIONS==========




//--------------------LED LIGHTS--------------------LED LIGHTS--------------------LED LIGHTS--------------------


//Start Up Lights
void StartUpLights()
{
  for (byte pin = A0; pin < A0 + 6; pin++) {      // Setting all pin modes to output via for loop
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // Startup light show
  for (int j = 0; j <= 3; j++) {
    for (byte pin = A0; pin < A0 + 6; pin++) {
      digitalWrite(pin, LOW);
      delay(80);
      digitalWrite(pin, HIGH);
    }
  }
  delay(300);
  for (byte pin = A5; pin >= A0; pin--) {
    digitalWrite(pin, LOW);
    delay(100);
  }
}

//MOVE FORWARD ANIMATION (ALL LIGHTS OFF)
void LightsOut()
{
  for (byte pin = A5; pin >= A0; pin--) {
    digitalWrite(pin, LOW);
  }
}


//--------------------IMU SENSOR--------------------IMU SENSOR--------------------IMU SENSOR--------------------


// IMU Direction function
// This function gets the current direction from the IMU
float IMUDirection()
{

  for (int i = 0; i < 20; i++) {                                         // Looping 20 times just to get a good average value
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    thetaM = -atan2(accel.x() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360; // Math to get the pitch from the accelerometer
    phiM = -atan2(accel.y() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360; // Math to get the roll from the accelerometer

    phiFnew = .95 * phiFold + .05 * phiM;                             // Filter for accelerometer roll data
    thetaFnew = .95 * thetaFold + .05 * thetaM;                       // Filter for accelerometer pitch data

    dt = (millis() - millisOld) / 1000.;                              // Measuring the change in time since last millis measurement
    millisOld = millis();                                             // Setting millisOld to millis to update it for next calculation
    theta = (theta + gyro.y() * dt) * .95 + thetaM * .05;             // Calculating Absolute Roll using fancy math (and running a filter on it)
    phi = (phi - gyro.x() * dt) * .95 + phiM * .05;                   // Calculating Absolute Pitch using fancy math  (and running a filter on it)
   
    phiRad = phi / 360 * (2 * 3.14);                                  // Calculating
    thetaRad = theta / 360 * (2 * 3.14);

    Xm = mag.x() * cos(thetaRad) - mag.y() * sin(phiRad) * sin(thetaRad) + mag.z() * cos(phiRad) * sin(thetaRad); // Absolute X Direction of magnitometer after fancy math
    Ym = mag.y() * cos(phiRad) + mag.z() * sin(phiRad);                                                 // Absolute Y Direction of magnitometer after fancy math

    psi = atan2(Ym, Xm) / (2 * 3.14) * 360;                           // Absolute heading of the robot

    phiFold = phiFnew;
    thetaFold = thetaFnew;
  }                                 // End of for loop
  return psi;     // Return the absolute heading 
}



// IMU Pitch function
// This function gets the current pitch angle from the IMU
float IMUPitch()
{
  for (int i = 0; i < 20; i++) {                                         // Looping 20 times just to get a good average value
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    thetaM = -atan2(accel.x() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360; // Math to get the pitch from the accelerometer
    phiM = -atan2(accel.y() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360; // Math to get the roll from the accelerometer

    phiFnew = .95 * phiFold + .05 * phiM;                             // Filter for accelerometer roll data
    thetaFnew = .95 * thetaFold + .05 * thetaM;                       // Filter for accelerometer pitch data

    dt = (millis() - millisOld) / 1000.;                              // Measuring the change in time since last millis measurement
    millisOld = millis();                                             // Setting millisOld to millis to update it for next calculation
    theta = (theta + gyro.y() * dt) * .95 + thetaM * .05;             // Calculating Absolute Roll using fancy math (and running a filter on it)
    phi = (phi - gyro.x() * dt) * .95 + phiM * .05;                   // Calculating Absolute Pitch using fancy math  (and running a filter on it)
    
    phiFold = phiFnew;
    thetaFold = thetaFnew;
  }                                 // End of for loop
  return phi;     // Return the absolute pitch 
}


//--------------------MOVEMENT--------------------MOVEMENT-------------------MOVEMENT-------------------
//TURN LEFT
void TurnLeft(int deg)
{

  Serial2.println(" ");
  Serial2.println("=====TurnLeft Function Successfully Called===== ");        // Printing for testing
  
  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds 
  int NumLed = 0;                         // Initalizing NumLed and setting it to 0 for blinker functions
  float currentDirection = 0;             // Creating local variable named currentDirection and setting it to 0
  float startingDirection = 0;            // Creating local variable named startingDirection and setting it to 0
  float targetDirection = 0;              // Creating local variable named targetDirection and setting it to 0
  float tempValue = 0;                    // Creating variable named x for use in calculating targetDirection (holds a value while another variable is rewritten)

  startingDirection = IMUDirection();         // setting startingDirection to the current value returned from the function IMUDirection()
  targetDirection = startingDirection - deg;  // setting targetDirction = to the startingDirection - the deg value we passed to this function 


  Serial2.print(" Starting Direction: ");     // Printing for testing
  Serial2.println(startingDirection);

  if (targetDirection < -180) {               // Calculating the target position if it goes over the -180 mark
    tempValue = -targetDirection - 180;
    targetDirection = 180 - tempValue;
  }

  Serial2.print(" Target Direction: ");       // Printing for testing
  Serial2.println(targetDirection);

  Serial2.println("Begin Turning Left");
  currentDirection = IMUDirection();          // currentDirection is being updated to the value returned by the IMUDirection function 
  

  while (currentDirection > targetDirection + 9 || currentDirection < targetDirection - 9) {    // This while loop repeates untill the curentDirection has reached the target direction
    unsigned long currentTime = millis(); 
    if (currentTime - prevTime > 150) {
      switch (NumLed) {
        case 0:
          digitalWrite(A2, HIGH);
          NumLed = 1;
          prevTime = currentTime;
          break;

        case 1:
          digitalWrite(A1, HIGH);
          NumLed = 2;
          prevTime = currentTime;
          break;

        case 2:
          digitalWrite(A0, HIGH);
          NumLed = 3;
          prevTime = currentTime;
          break;

        case 3:
          digitalWrite(A2, LOW);
          NumLed = 4;
          prevTime = currentTime;
          break;

        case 4:
          digitalWrite(A1, LOW);
          NumLed = 5;
          prevTime = currentTime;
          break;

        case 5:
          digitalWrite(A0, LOW);
          NumLed = 0;
          prevTime = currentTime;
          break;
      }
    }

      // Set speed of motors. Some are faster than others to stop turning malfunctions
  analogWrite(DCmotorFrontPWMA, 255);   // Front Right
  analogWrite(DCmotorFrontPWMB, 255);   // Front Left
  analogWrite(DCmotorMiddlePWMA, 255);  // Middle Right
  analogWrite(DCmotorMiddlePWMB, slow);  // Middle Left
  analogWrite(DCmotorBackPWMA, slow);    // Back Right
  analogWrite(DCmotorBackPWMB, 255);    // Back Left

  // All Right Motors Move Forward
  digitalWrite(DCmotorFrontAI1, LOW);   // Front Right Forwards
  digitalWrite(DCmotorFrontAI2, HIGH);
  digitalWrite(DCmotorMiddleAI1, LOW);  // Middle Right Forwards
  digitalWrite(DCmotorMiddleAI2, HIGH);
  digitalWrite(DCmotorBackAI1, LOW);    // Back Right Forwards
  digitalWrite(DCmotorBackAI2, HIGH);

  // All Left Motors Move Backwards
  digitalWrite(DCmotorFrontBI1, HIGH);       // Front Left Backwards
  digitalWrite(DCmotorFrontBI2, LOW);
  digitalWrite(DCmotorMiddleBI1, HIGH);      // Middle Left Backwards
  digitalWrite(DCmotorMiddleBI2, LOW);
  digitalWrite(DCmotorBackBI1, HIGH);        // Back Left Backwards
  digitalWrite(DCmotorBackBI2, LOW);

  currentDirection = IMUDirection();
  Serial2.print("Target Direction: ");
  Serial2.print(targetDirection);
  Serial2.print("Current Direction: ");
  Serial2.println(currentDirection);

  }
  DCStop();
  LightsOut();
  Serial2.println(" Left Turn Complete ");
  Serial2.println("----------LeftTurn Function Complete----------");
  Serial2.println(" ");
  delay(MovementDelay);
}


//TURN RIGHT
void TurnRight(int deg)
{
  Serial2.println(" ");
  Serial2.println("=====TurnRight Function Successfully Called===== ");
  
  unsigned long prevTime = millis();
  int NumLed = 0;
  float currentDirection = 0;
  float startingDirection = 0;
  float targetDirection = 0;
  

  startingDirection = IMUDirection();
  targetDirection = startingDirection + deg;

  Serial2.print(" Starting Direction: ");
  Serial2.println(startingDirection);

  if (targetDirection > 180) {     // Calculating the target position if it goes over the -180 mark
    deg = -targetDirection + 180;
    targetDirection = -180 - deg;
  }

  Serial2.print("Target Direction: ");
  Serial2.println(targetDirection);

  Serial2.println("Begin Turning Right");
  currentDirection = IMUDirection();

 

  while (currentDirection > targetDirection + 9 || currentDirection < targetDirection - 9) {
    unsigned long currentTime = millis();
    if (currentTime - prevTime > 150) {
      switch (NumLed) {
        case 0:
          digitalWrite(A3, HIGH);
          NumLed = 1;
          prevTime = currentTime;
          break;

        case 1:
          digitalWrite(A4, HIGH);
          NumLed = 2;
          prevTime = currentTime;
          break;

        case 2:
          digitalWrite(A5, HIGH);
          NumLed = 3;
          prevTime = currentTime;
          break;

        case 3:
          digitalWrite(A3, LOW);
          NumLed = 4;
          prevTime = currentTime;
          break;

        case 4:
          digitalWrite(A4, LOW);
          NumLed = 5;
          prevTime = currentTime;
          break;

        case 5:
          digitalWrite(A5, LOW);
          NumLed = 0;
          prevTime = currentTime;
          break;
      }
    }
     // Set speed of motors. Some are faster than others to stop turning malfunctions
    analogWrite(DCmotorFrontPWMA, 255);   // Front Right
    analogWrite(DCmotorFrontPWMB, 255);   // Front Left
    analogWrite(DCmotorMiddlePWMA, slow);  // Middle Right
    analogWrite(DCmotorMiddlePWMB, 255);  // Middle Left
    analogWrite(DCmotorBackPWMA, 255);    // Back Right
    analogWrite(DCmotorBackPWMB, slow);    // Back Left
    
    // All Right Motors Move Backwards
    digitalWrite(DCmotorFrontAI1, HIGH);   // Front Right Backwards
    digitalWrite(DCmotorFrontAI2, LOW);
    digitalWrite(DCmotorMiddleAI1, HIGH);  // Middle Right Backwards
    digitalWrite(DCmotorMiddleAI2, LOW);
    digitalWrite(DCmotorBackAI1, HIGH);    // Back Right Backwards
    digitalWrite(DCmotorBackAI2, LOW);
    
    // All Left Motors Move Forward
    digitalWrite(DCmotorFrontBI1, LOW);       // Front Left Forward
    digitalWrite(DCmotorFrontBI2, HIGH);
    digitalWrite(DCmotorMiddleBI1, LOW);      // Middle Left Forward
    digitalWrite(DCmotorMiddleBI2, HIGH);
    digitalWrite(DCmotorBackBI1, LOW);        // Back Left Forward
    digitalWrite(DCmotorBackBI2, HIGH);

    currentDirection = IMUDirection();
    Serial2.print("Target Direction: ");
    Serial2.print(targetDirection);
    Serial2.print("Current Direction: ");
    Serial2.println(currentDirection);
  }
  DCStop();
  LightsOut();
  Serial2.println(" Right Turn Complete ");
  Serial2.println("----------RightTurn Function Complete----------");
  Serial2.println(" ");
  delay(MovementDelay);
}
  

//DC MOTORS
void MoveForward()
{
  Serial2.println("Moving forward");
  LightsOut();

  // Set speed of all motors to 100%
  digitalWrite(DCmotorFrontPWMA, HIGH);     // Front Right Motor
  digitalWrite(DCmotorFrontPWMB, HIGH);     // Front Left Motor
  digitalWrite(DCmotorMiddlePWMA, HIGH);    // Middle Right Motor
  digitalWrite(DCmotorMiddlePWMB, HIGH);    // Middle Left Motor
  digitalWrite(DCmotorBackPWMA, HIGH);      // Back Right Motor
  digitalWrite(DCmotorBackPWMB, HIGH);      // Back Left Motor

  // All Right Motors move forward
  digitalWrite(DCmotorFrontAI1, LOW);       // Front Right Forward
  digitalWrite(DCmotorFrontAI2, HIGH);      
  digitalWrite(DCmotorMiddleAI1, LOW);      // Middle Right Forward
  digitalWrite(DCmotorMiddleAI2, HIGH);
  digitalWrite(DCmotorBackAI1, LOW);        // Back Right Forward
  digitalWrite(DCmotorBackAI2, HIGH);

  // All Left Motors move forward
  digitalWrite(DCmotorFrontBI1, LOW);       // Front Left Forward
  digitalWrite(DCmotorFrontBI2, HIGH);
  digitalWrite(DCmotorMiddleBI1, LOW);      // Middle Left Forward
  digitalWrite(DCmotorMiddleBI2, HIGH);
  digitalWrite(DCmotorBackBI1, LOW);        // Back Left Forward
  digitalWrite(DCmotorBackBI2, HIGH);

}

// DC MOTORS
void MoveBack()
{
  int NumLed = 1;
  unsigned long targetTime = millis();
  unsigned long currentTime = millis();
  unsigned long prevTime = millis();
  while (currentTime - targetTime < 3000) {
    currentTime = millis();
    if (currentTime - prevTime > 200) {
      switch (NumLed) {
        case 0:
          for (byte pin = A5; pin >= A0; pin--) {
            digitalWrite(pin, HIGH);
          }
          NumLed = 1;
          break;

        case 1:
          for (byte pin = A5; pin >= A0; pin--) {
            digitalWrite(pin, LOW);
          }
          NumLed = 0;
          break;

      }
      prevTime = currentTime;
    }

  // Set speed of all motors to 100%
  digitalWrite(DCmotorFrontPWMA, HIGH);     // Front Right Motor
  digitalWrite(DCmotorFrontPWMB, HIGH);     // Front Left Motor
  digitalWrite(DCmotorMiddlePWMA, HIGH);    // Middle Right Motor
  digitalWrite(DCmotorMiddlePWMB, HIGH);    // Middle Left Motor
  digitalWrite(DCmotorBackPWMA, HIGH);      // Back Right Motor
  digitalWrite(DCmotorBackPWMB, HIGH);      // Back Left Motor

  // Move All Right Motors Backwards 
  digitalWrite(DCmotorFrontAI1, HIGH);      // Front Right Backwards
  digitalWrite(DCmotorFrontAI2, LOW);
  digitalWrite(DCmotorMiddleAI1, HIGH);     // Middle Right Backwards
  digitalWrite(DCmotorMiddleAI2, LOW);
  digitalWrite(DCmotorBackAI1, HIGH);       // Back Right Backwards
  digitalWrite(DCmotorBackAI2, LOW);

  // Move All Left Motors Backwards
  digitalWrite(DCmotorFrontBI1, HIGH);      // Front Left Backwards
  digitalWrite(DCmotorFrontBI2, LOW);
  digitalWrite(DCmotorMiddleBI1, HIGH);     // Middle Left Backwards
  digitalWrite(DCmotorMiddleBI2, LOW);
  digitalWrite(DCmotorBackBI1, HIGH);       // Back Left Backwards
  digitalWrite(DCmotorBackBI2, LOW);
  }
}

void DCStop()
{
  // Set speed of all motors to 0% 
  digitalWrite(DCmotorFrontPWMA, LOW);
  digitalWrite(DCmotorFrontPWMB, LOW);
  digitalWrite(DCmotorMiddlePWMA, LOW);
  digitalWrite(DCmotorMiddlePWMB, LOW);
  digitalWrite(DCmotorBackPWMA, LOW);
  digitalWrite(DCmotorBackPWMB, LOW);

  for (int j = 0; j <= 1; j++) {
    for (byte pin = A5; pin >= A0; pin--) {
      digitalWrite(pin, HIGH);
    }
    delay(100);
    for (byte pin = A5; pin >= A0; pin--) {
      digitalWrite(pin, LOW);
    }
    delay(100);
  }
  for (byte pin = A5; pin >= A0; pin--) {
    digitalWrite(pin, HIGH);
  }
}


// Time of Flight Distance Getter
float GetDistance(){
  float OldDistance = distance;
  VL53L0X_RangingMeasurementData_t measure;                                         // create TOF variable named measure
    
  Serial2.println("GetDistance Function Successfully Called");    
  DarwinTOF.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeMilliMeter < 8190)   // 8190 is the value returned when the TOF sensor measurements are out of range
  {
    distance = measure.RangeMilliMeter;
    Serial2.print("Distance (mm): "); Serial2.println(distance);       // measure.RangeMilliMeter is the actual variable we will need
    
  } 
  else 
  {
    distance = OldDistance;
    Serial2.print(" Out of Range: using OldDistance value : ");
    Serial2.println(distance);
    
  } 
  
  return distance;
}
