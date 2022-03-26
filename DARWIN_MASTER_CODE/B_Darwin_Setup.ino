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
  int8_t temp = DarwinIMU.getTemp();      // int8_t is a special type of int variable type that stores values from -120 to 120. super compact. Some of the measurements from the IMU are dependant on tempature. So we need to measure the tempature of the IMU first. So we will create a new variable named temp of type int8_t and set it = to a function return of the imu library that does exactly that.*/
  Serial.print("IMU Tempature: ");        // Printing for debugging
  Serial.println(temp);                   // Printing the measured tempature to the screen for sanity check.
  CalibrateIMU();
  DarwinIMU.setExtCrystalUse(true);       // Dont use the crystal on the chip itself, use crystal on the board (for time keeping)
  millisOld = millis();                   // Grabbing the system time for dt variable
  Serial.println("Calling IMU Calibration Function");   // Printing for debugging
  Serial.println(" ");
  Serial.println("IMU Calibration Complete");   // Printing for debugging
  Serial.println(" ");

  GetGoldenDirection();
  
  // End of Void Setup
  Serial.println("End of Void Setup");          // Printing for debugging
  Serial.println(" ");                          // Printing for debugging

}
