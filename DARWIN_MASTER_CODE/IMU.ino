bool Navigation(float dir){
  if(IMUDirection() > dir - 10  && IMUDirection() < dir + 10){
    return true;  
  }
  else
  {
    return false;
  }
}



// IMU Direction function
// This function gets the current direction from the IMU
float IMUDirection()
{

  for (int i = 0; i < 10; i++) {                                         // Looping 20 times just to get a good average value
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
  for (int i = 0; i < 10; i++) {                                         // Looping 20 times just to get a good average value
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

    phiFold = phiFnew;
    thetaFold = thetaFnew;
  }                                 // End of for loop
  return theta;     // Return the absolute pitch
}

void CalibrateIMU(){
  Serial2.println(" ");
  Serial2.println("==========SetupIMU Function Successfully Called==========");
  uint8_t system, gyroCal, accelCal, magCal = 0;

  while (accelCal < 3) {
    DarwinIMU.getCalibration(&system, &gyroCal, &accelCal, &magCal);
    imu::Vector<3> accel = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = DarwinIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    Serial2.print("Accelerometer: \t");
    Serial2.print(accelCal);

    Serial2.print("\t   Gyro: \t");
    Serial2.print(gyroCal);

    Serial2.print("\t   Magnetometer: \t");
    Serial2.print(magCal);
    
    Serial2.print("\tsystem: \t");
    Serial2.println(system);
  }

  Serial2.println("IMU Calibration Complete");
  Serial2.println("----------IMU Setup Function Complete----------");
  Serial2.println(" ");
}



float GetGoldenDirection(){

  float x;
  float y;
  float z;
  float avg;

  Serial2.println("Place Robot on the ground and prepare for run");
  Serial2.println("Taking Golden direction in: ");
  
  for(int i = 30; i > 0; i--){
    Serial2.println(i);
    delay(1000);      
    }

  
  x = IMUDirection();
  delay(1000);
  y = IMUDirection();
  delay(1000);
  z = IMUDirection();

  avg = (x + y + z)/3;
  
  GoldenDirection = avg; 
  Serial2.print("Golden Direction set to: ");
  Serial2.println(GoldenDirection); 
}
