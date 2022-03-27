bool ObjectDetection() {

  int distance = 0;
  int minDistance = 330;

  distance = GetDistance();
  if (distance > minDistance) {
//    Serial2.print("ObjectDetection() = FALSE \t Distance = \t");
//    Serial2.print(distance);
//    Serial2.print("\t");
    return false;
  }
  else
  {
//    Serial2.print("ObjectDetection() = TRUE \t Distance = \t");
//    Serial2.print(distance);
//    Serial2.print("\t");
    return true;
  }
}

void MeasureObject() {
  int AdjacentPos = 0;

  if (stepperAngle > 0) {
    while (stepperAngle != 0) {
      StepperLeft(1);
      stepperAngle = stepperAngle - 1;
    }
  }
  else
  {
    while (stepperAngle != 0) {
      StepperRight(1);
      stepperAngle = stepperAngle + 1;
    }
  }
  LevelTOF();
  delay(50);
  LevelTOF();
  delay(50);
  AdjacentPos = GetDistance();
  while (GetDistance() < 8000) {

  }


  // center stepper
  // center servo
  // measure straight out in front
  // measure up until infinity is reached
}


// Uses the TOF sensor to measure a distance
int GetDistance() {

  int distance = 0;                                                   // Creating and INT and naming it distance, to store the distance value recieved form the TOF sensor.

  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  distance = measure.RangeMilliMeter;

  return distance;                                                // Return the value stored in distance
}                                                                 // End of GetDistance()


float ServoPos(int deg) {
  TOFServo.write(DegToServo(deg));
}

void LevelTOF() {
  int deg = 0;
  deg = DegToServo(IMUPitch() + 80);
  TOFServo.write(deg);
}


// Convert servos position value to degrees above ground level
float ServoToDeg(int servoPos) {                                  // Send this function the servos coded position
  float deg = 0;                                                      // Create a FLOAT named deg to store our value in degrees
  deg = -0.4875 * servoPos + 133.875;                             // y = mx+b math to calculate the real value in degrees the servo is set to.
  return deg;                                                     // Return the value of degrees as a float
}                                                                 // End of ServoToDeg()


// Convers a degree above ground level to a real value to set the servo to
float DegToServo(float deg) {                                     // Send this function the degrees above ground level you wish to set the servo to
  int servoPos = 0;                                                   // Create an INT named servoPos to store our value of the servo in
  servoPos = (deg - 133.875) / -0.4875;                           // x = (y-b)/m math to calculate the value to send servo to achieve a speciffic degree
  return servoPos;                                                // Return the value to set the servo to
}                                                             // End of DegToServo()
