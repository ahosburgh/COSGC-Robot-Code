void setup() {

  //Bluetooth Serial2 Communication
  Serial2.begin(9600);                                                 // Begin bluetooth communication at 9600
  Serial2.println("Serial2.begin(9600)\t Bluetooth Serial Communication Established\n");    // Confirm bluetooth connection established
  // Pin TX2 and RX2 -- Yellow and Blue

  //TOF Sensor Setup
  DarwinTOF.begin();                         // Establish connection with TOF sensor
  Serial2.println("DarwinTOF.begin(); \t Time Of Flight Sensor Connection Established\n");   // Printing for debugging

  //TOF Stepper Setup
  TOFStepper.setSpeed(StepperSpeed);         // Setting TOF Stepper Motor speed
  Serial2.println("TOFStepper.setSpeed(StepperSpeed); \t Front Stepper Motor Speed Set\n");

  //Servo (TOF) Setup
  TOFServo.attach(TOFServoPin);                 // Attaches the servo to the signalPin of the Arduino
  Serial2.println("TOFServo.attach(TOFServoPin); \t Front Servo Motor Connection Pin Set\n");         // Printing for debugging
  TOFServo.write(TOFServoPos);                  // Set servo to start position (90 degrees)
  Serial2.println("TOFServo.write(TOFServoPos); \t Set Servo to Inital Starting Position\n");    // Printing for debugging


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
  Serial2.println("All DC Motor pins set to OUTPUT\n");

  //LED Tail Lights Setup
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(Led5, OUTPUT);
  pinMode(Led6, OUTPUT);
  Serial2.println("All LED Pins set to OUTPUT\n");
  Serial2.println("Running StartUpLights()\n");
  StartUpLights();


  //IMU Sensor Setup
  DarwinIMU.begin();                      // Starting the IMU waiting 1 second to give it time to power on and make its connection before sending another command
  delay(1000);                            // waiting 1 second to give it time to power on and make its connection before sending another command
  /* Initialise the sensor */
  if (!DarwinIMU.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial2.print("Ooops, no BNO055 detected ... Restart");
    while (1);
  }
  Serial2.println("DarwinIMU.begin(); \t IMU Connection Established\n");
  int8_t temp = DarwinIMU.getTemp();      // int8_t is a special type of int variable type that stores values from -120 to 120. super compact. Some of the measurements from the IMU are dependant on tempature. So we need to measure the tempature of the IMU first. So we will create a new variable named temp of type int8_t and set it = to a function return of the imu library that does exactly that.*/
  Serial2.print("IMU Tempature: ");       // Printing for debugging
  Serial2.println(temp);                  // Printing the measured tempature to the screen for sanity check.


  Serial2.println("setCal(); \t Loading Calibration Offsets\n");   // Printing for debugging
  setCal();

  DarwinIMU.setExtCrystalUse(true);       // Dont use the crystal on the chip itself, use crystal on the board (for time keeping)
  millisOld = millis();                   // Grabbing the system time for dt variable

  Serial2.println("GetGoldenDirection(); \t Calling Get Golden Direction Function \n");
  GetGoldenDirection();
  CenterStepper();
  
  Serial2.println("Main Program is About to Begin");
}
