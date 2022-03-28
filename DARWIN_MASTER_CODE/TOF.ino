void CenterStepper(){

  ServoPos(0);
  
  
}

void Avoidence() {
  int LeftDist = 0;
  int RightDist = 0;
  int LeftAng = 0;
  int RightAng = 0;
  int FrontDist = 0;

  LevelTOF();
  Serial2.println("\n ======== Avoidence Function Successfully Called ======== \n");

  Serial2.println("Centering Stepper\n");
  Serial2.print("Stepper Position: \t");
  Serial2.println(stepperAngle);
  if (stepperAngle > 0) {
    while (stepperAngle != 0) {
      StepperLeft(1);
      stepperAngle = stepperAngle - 1;
      Serial2.print("Stepper Angle: \t");
      Serial2.println(stepperAngle);
    }
  }
  else
  {
    while (stepperAngle != 0) {
      StepperRight(1);
      stepperAngle = stepperAngle + 1;
      Serial2.print("Stepper Angle: \t");
      Serial2.println(stepperAngle);
    }
  }

  Serial2.println("\nMeasuring Objects Width: \n");

  FrontDist = GetDistance();
  Serial2.print("Front measurement: \t");
  Serial2.println(FrontDist);

  while (stepperAngle < 90 && LeftDist < 8000) {
    stepperAngle = stepperAngle + 1;
    StepperLeft(1);
    LeftDist = GetDistance();
    Serial2.print("StepperAngle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tLeftDist:\t");
    Serial2.println(LeftDist);
  }

  if (LeftDist > 8000 && stepperAngle > 0) {
    Serial2.println("Object Edge Detected. Reversing Direction");
    while (LeftDist > 8000) {
      stepperAngle = stepperAngle - 1;
      StepperRight(1);
      LeftDist = GetDistance();
      Serial2.print("StepperAngle:\t");
      Serial2.print(stepperAngle);
      Serial2.print("\tLeftDist:\t");
      Serial2.println(LeftDist);
    }
  }

  Serial2.print("\nLeft Distance Final:\t");
  Serial2.print(LeftDist);
  LeftAng = stepperAngle;
  Serial2.print("\tLeft Angle Final:\t");
  Serial2.println(LeftAng);
  StepperRight(stepperAngle);
  stepperAngle = 0;
  delay(1000);

  while (stepperAngle < 90 && GetDistance() < 8000) {
    stepperAngle = stepperAngle + 1;
    StepperRight(1);
    RightDist = GetDistance();
    Serial2.print("StepperAngle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tRightDist:\t");
    Serial2.println(RightDist);
  }

  if (RightDist > 8000 && stepperAngle > 0) {
    Serial2.println("Object Edge Detected. Reversing Direction");
    while (RightDist > 8000) {
      stepperAngle = stepperAngle - 1;
      StepperLeft(1);
      RightDist = GetDistance();
      Serial2.print("StepperAngle:\t");
      Serial2.print(stepperAngle);
      Serial2.print("\tRightDist:\t");
      Serial2.println(RightDist);
    }
  }


  Serial2.print("\nRight Distance Final:\t");
  Serial2.print(RightDist);
  RightAng = stepperAngle;
  Serial2.print("\tRight Angle Final:\t");
  Serial2.println(RightAng);
  
  StepperLeft(stepperAngle);/////////////////////////////////////////
  stepperAngle = 0;//////////////////////////////////////////////////replace with "Center Stepper" Function

  int LeftWidth;
  LeftWidth = sqrt(sq(LeftDist) - sq(FrontDist));
  int RightWidth;
  RightWidth = sqrt(sq(RightDist) - sq(FrontDist));

  Serial2.print("Left Width: \t");
  Serial2.print(LeftWidth);
  Serial2.print("\tRight Width: \t");
  Serial2.println(RightWidth);

  int Width = LeftWidth + RightWidth;

  Serial2.print("\t Total Width Of Object:\t");
  Serial2.println(Width);

  if (RightAng > LeftAng) {
    Serial2.print("Left Side appears more clear. Turning Left:\t");
    Serial2.print(LeftAng + 10);
    Serial2.print(" degrees.");
    DCBack(1000);
    TurnLeft(LeftAng + 10);
    MoveForward(LeftAng + 10);
  }
  else
  {
    Serial2.print("Right Side appears more clear. Turning Right:\t");
    Serial2.print(RightAng + 10);
    Serial2.print(" degrees.");
    DCBack(1000);
    TurnRight(RightAng + 10);
    MoveForward(RightAng + 10);
  }
}


bool ObjectDetection() {

  int distance = 0;
  int minDistance = 500;

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
  int AdjAng =  0;
  int AdjDist = 0;
  int OppDist = 0;
  double ObjHeight = 0;

  Serial2.print("Stepper Angle: \t");
  Serial2.println(stepperAngle);
  if (stepperAngle > 0) {
    while (stepperAngle != 0) {
      StepperLeft(1);
      stepperAngle = stepperAngle - 1;
      Serial2.print("Stepper Angle: \t");
      Serial2.println(stepperAngle);
    }
  }
  else
  {
    while (stepperAngle != 0) {
      StepperRight(1);
      stepperAngle = stepperAngle + 1;
      Serial2.print("Stepper Angle: \t");
      Serial2.println(stepperAngle);
    }
  }
  LevelTOF();
  AdjDist = GetDistance();
  AdjAng = DegToServo(IMUPitch() + 80);
  while (GetDistance() < 8000 || AdjAng < 180) {
    ServoPos(AdjAng);
    OppDist = GetDistance();
    AdjAng = AdjAng + 1;
  }

  ObjHeight = 220 + (sqrt(sq(OppDist) - sq(AdjDist)));
  Serial2.print(" Object Height: \t");
  Serial2.println(ObjHeight / 10);
  LevelTOF();
  delay(8000);
}


// Uses the TOF sensor to measure a distance
int GetDistance() {
  int dist1 = 0;
  int dist2 = 0;
  int dist3 = 0;
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
