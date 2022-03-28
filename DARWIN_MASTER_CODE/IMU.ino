void Navigation(float dir) {
  int tempValue = 0;
  float currentDirection = IMUDirection();
  float targetDirection = 0;
  Serial2.println("\n ======== Navigation Function Successfully Called ======== \n");
  Serial2.print("Navigation Check On: \t");
  Serial2.print(dir);
  int switchCase = 0;
  if (dir < -170) {
    switchCase = 0;
  }
  else if (dir > 170) {
    switchCase = 1;
  }
  else if (dir <= 170 && dir >= -170)
  {
    switchCase = 2;
  }
  switch (switchCase)
  {
    case 0:
      tempValue = dir + 350;
      if (currentDirection < dir + 10 || currentDirection > tempValue)
      {
        Serial2.print("\tIMU reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");

        tempValue = dir + 180;
        if (currentDirection > tempValue) {
          targetDirection = 180 - currentDirection + tempValue;
          Serial2.print("Turning Right \t") ;
          Serial2.println(targetDirection);
          TurnRight(targetDirection);
        }
        else if (currentDirection < tempValue) {
          targetDirection = -1 * (dir - currentDirection);
          Serial2.print("Turning Left \t") ;
          Serial2.println(targetDirection);
          TurnLeft(targetDirection);
        }
      }
      break;


    case 1:
      tempValue = dir - 350;
      if (currentDirection > dir - 10 || currentDirection < tempValue)
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");
        tempValue = dir - 180;
        if (currentDirection > tempValue) {
          targetDirection = 180 - currentDirection + tempValue;
          Serial2.print("Turning Right \t") ;
          Serial2.println(targetDirection);
          TurnRight(targetDirection);
        }
        else if (currentDirection < tempValue) {
          targetDirection = -1 * (dir - currentDirection);
          Serial2.print("Turning Left \t") ;
          Serial2.println(targetDirection);
          TurnLeft(targetDirection);
        }
      }


    case 2:
      if (currentDirection > dir - 10 && currentDirection < dir + 10)
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");
        if (dir < 0) {
          tempValue = dir + 180;
        }
        if (dir > 0) {
          tempValue = dir - 180;
        }
        if (currentDirection > tempValue) {
          targetDirection = 180 - currentDirection + tempValue;
          Serial2.print("Turning Right \t") ;
          Serial2.println(targetDirection);
          TurnRight(targetDirection);
        }
        else if (currentDirection < tempValue) {
          targetDirection = -1 * (dir - currentDirection);
          Serial2.print("Turning Left \t") ;
          Serial2.println(targetDirection);
          TurnLeft(targetDirection);
        }
        break;
      }
  }
}

// IMU Roll function
// This function gets the current Roll from the IMU
float IMURoll()
{

  for (int i = 0; i < 10; i++) {                                         // Looping 10 times just to get a good average value
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    phiM = -atan2(accel.y() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360;    // Math to get the roll from the accelerometer

    phiFnew = .95 * phiFold + .05 * phiM;                             // Filter for accelerometer roll data

    dt = (millis() - millisOld) / 1000.;                              // Measuring the change in time since last millis measurement
    millisOld = millis();                                             // Setting millisOld to millis to update it for next calculation
    phi = (phi - gyro.x() * dt) * .95 + phiM * .05;                   // Calculating Absolute Pitch using fancy math  (and running a filter on it)

    phiRad = phi / 360 * (2 * 3.14);                                  // Calculating

    phiFold = phiFnew;

  }                                 // End of for loop
  return phi;     // Return the absolute roll
}

// IMU Direction function
// This function gets the current direction from the IMU
float IMUDirection()
{

  for (int i = 0; i < 20; i++) {                                         // Looping 10 times just to get a good average value
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    thetaM = -atan2(accel.x() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360;  // Math to get the pitch from the accelerometer
    phiM = -atan2(accel.y() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360;    // Math to get the roll from the accelerometer

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
  for (int i = 0; i < 10; i++) {                                         // Looping 10 times just to get a good average value
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    thetaM = -atan2(accel.x() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360;  // Math to get the pitch from the accelerometer
    thetaFnew = .95 * thetaFold + .05 * thetaM;                       // Filter for accelerometer pitch data
    dt = (millis() - millisOld) / 1000.;                              // Measuring the change in time since last millis measurement
    millisOld = millis();                                             // Setting millisOld to millis to update it for next calculation
    theta = (theta + gyro.y() * dt) * .95 + thetaM * .05;             // Calculating Absolute Roll using fancy math (and running a filter on it)
    thetaFold = thetaFnew;
  }                                 // End of for loop
  return theta;     // Return the absolute pitch
}

void CalibrateIMU() {
  Serial2.println("\n==========CalibrateIMU Function==========\n");
  Serial2.println("Rotate, Twist, and Invert Darwin Until Values For All Axis's = 3\n");
  delay(1000);

  uint8_t system, gyroCal, accelCal, magCal = 0;

  while (accelCal < 3) {
    DarwinIMU.getCalibration(&system, &gyroCal, &accelCal, &magCal);
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    Serial2.print("Accel: \t");
    Serial2.print(accelCal);

    Serial2.print("\tGyro: \t");
    Serial2.print(gyroCal);

    Serial2.print("\tMag: \t");
    Serial2.print(magCal);

    Serial2.print("\tSystem: \t");
    Serial2.println(system);

    if (x == true) {
      if (stepperAngle < 50)
      {
        stepperAngle = stepperAngle + 50;
        StepperLeft(50);
      }
      else
      {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -50)
      {
        stepperAngle = stepperAngle - 50;
        StepperRight(50);
      }
      else
      {
        x = true;
      }
    }
  }
  while (IMUDirection() == 0)
  {
    Serial2.print("Calibration Failed -- Restart -- \t");
    Serial2.println(IMUDirection());
  }
  Serial2.println("\n==========IMU Calibration Successful==========\n");
  delay(3000);
}



float GetGoldenDirection() {
  Serial2.println("\n==========Golden Direction==========\n");
  Serial2.println("Place Darwin on the ground and prepare for run");
  delay(3000);

  float sample1 = 0;
  float sample2 = 0;
  float sample3 = 0;
  int i = 9;
  unsigned long currentTime = millis();
  unsigned long oldTime = millis();

  Serial2.println("Taking Golden Direction Samples In: ");
  while (i > 0) {
    if (currentTime - oldTime > 1000)
    {
      Serial2.print("------------------------------\t");
      Serial2.println(i);
      i = i - 1;
      oldTime = currentTime;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();
    if (x == true)
    {
      if (stepperAngle < 50)
      {
        stepperAngle = stepperAngle + 50;
        StepperLeft(50);
      }
      else
      {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -50)
      {
        stepperAngle = stepperAngle - 50;
        StepperRight(50);
      }
      else
      {
        x = true;
      }
    }
  }
  currentTime = millis();
  oldTime = millis();
  i = 1;

  while (i > 0) {
    if (currentTime - oldTime > 1000) {
      sample1 = IMUDirection();
      Serial2.print("-------------------- Sample1: \t");
      Serial2.println(sample1);
      oldTime = currentTime;
      i = 0;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();

    if (x == true) {
      if (stepperAngle < 50) {
        stepperAngle = stepperAngle + 50;
        StepperLeft(50);
      }
      else {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -50) {
        stepperAngle = stepperAngle - 50;
        StepperRight(50);
      }
      else {
        x = true;
      }
    }
  }

  currentTime = millis();
  oldTime = millis();
  i = 1;

  while (i > 0) {
    if (currentTime - oldTime > 1000)
    {
      sample2 = IMUDirection();
      Serial2.print("-------------------- Sample2: \t");
      Serial2.println(sample2);
      oldTime = currentTime;
      i = 0;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();

    if (x == true)
    {
      if (stepperAngle < 50)
      {
        stepperAngle = stepperAngle + 50;
        StepperLeft(50);
      }
      else
      {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -50)
      {
        stepperAngle = stepperAngle - 50;
        StepperRight(50);
      }
      else
      {
        x = true;
      }
    }
  }

  currentTime = millis();
  oldTime = millis();
  i = 1;

  while (i > 0) {
    if (currentTime - oldTime > 1000)
    {
      sample3 = IMUDirection();
      Serial2.print("-------------------- Sample3: \t");
      Serial2.println(sample3);
      oldTime = currentTime;
      i = 0;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();

    if (x == true)
    {
      if (stepperAngle < 50)
      {
        stepperAngle = stepperAngle + 50;
        StepperLeft(50);
      }
      else
      {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -50)
      {
        stepperAngle = stepperAngle - 50;
        StepperRight(50);
      }
      else
      {
        x = true;
      }
    }
  }

  GoldenDirection = (sample1 + sample2 + sample3) / 3;

  if (GoldenDirection == 0) {
    Serial2.println("GoldenDirection = 0; \t Process Failed. \t Restart");
    while (1 > 0) {
      // do nothing
    }
  }
  else {
    Serial2.print("Golden Direction set to: ");
    Serial2.println(GoldenDirection);
    Serial2.println("Turn Stepper Motor To Face Forward\n");
    stepperAngle = 0;
    delay(9000);
  }
}
