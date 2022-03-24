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

  GoldenDirection = IMUDirection(); //Set heading for goal

}
