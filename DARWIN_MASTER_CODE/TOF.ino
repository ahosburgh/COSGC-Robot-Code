void Avoidence() {
  int LeftDist = 0;
  int RightDist = 0;
  int LeftAng = 0;
  int RightAng = 0;
  int FrontDist = 0;

  Serial2.println("\n ======== Avoidence Function Successfully Called ======== \n");

  CenterStepper();
  LevelTOF();
  Serial2.println("\nMeasuring Objects Width: \n");

  FrontDist = GetDistance();
  Serial2.print("Front measurement: \t");
  Serial2.println(FrontDist);
  Serial2.println("Measuring Left Side...");
  while (stepperAngle < 90 && LeftDist < 8000) {
    stepperAngle = stepperAngle + 1;
    StepperLeft(1);
    LeftDist = GetDistance();
    //    Serial2.print("StepperAngle:\t");
    //    Serial2.print(stepperAngle);
    //    Serial2.print("\tLeftDist:\t");
    //    Serial2.println(LeftDist);
  }

  Serial2.println("Object Edge Detected. Reversing Direction");
  while (LeftDist > 8000 && stepperAngle > 0) {
    stepperAngle = stepperAngle - 1;
    StepperRight(1);
    LeftDist = GetDistance();
    //      Serial2.print("StepperAngle:\t");
    //      Serial2.print(stepperAngle);
    //      Serial2.print("\tLeftDist:\t");
    //      Serial2.println(LeftDist);
  }

  Serial2.print("\nLeft Distance Final:\t");
  Serial2.print(LeftDist);
  LeftAng = stepperAngle;
  Serial2.print("\tLeft Angle Final:\t");
  Serial2.println(LeftAng);
  StepperRight(stepperAngle);
  stepperAngle = 0;
  delay(1000);

  while (stepperAngle < 90 && RightDist < 8000) {
    stepperAngle = stepperAngle + 1;
    StepperRight(1);
    RightDist = GetDistance();
    //    Serial2.print("StepperAngle:\t");
    //    Serial2.print(stepperAngle);
    //    Serial2.print("\tRightDist:\t");
    //    Serial2.println(RightDist);
  }

  Serial2.println("Object Edge Detected. Reversing Direction");
  while (RightDist > 8000 && stepperAngle > 0) {
    stepperAngle = stepperAngle - 1;
    StepperLeft(1);
    RightDist = GetDistance();
    //    Serial2.print("StepperAngle:\t");
    //    Serial2.print(stepperAngle);
    //    Serial2.print("\tRightDist:\t");
    //    Serial2.println(RightDist);
  }


  Serial2.print("\nRight Distance Final:\t");
  Serial2.print(RightDist);
  RightAng = stepperAngle;
  Serial2.print("\tRight Angle Final:\t");
  Serial2.println(RightAng);

  StepperLeft(RightAng);
  CenterStepper();
  LevelTOF();

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
    int tempDir = IMUDirection();
    for (int i = 0; i < 3; i++) {
      MoveForward(tempDir);
    }
  }
  else
  {
    Serial2.print("Right Side appears more clear. Turning Right:\t");
    Serial2.print(RightAng + 10);
    Serial2.print(" degrees.");
    DCBack(1000);
    TurnRight(RightAng + 10);
    int tempDir = IMUDirection();
    for (int i = 0; i < 3; i++) {
      MoveForward(tempDir);
    }
  }
}




void MeasureObject() {
  int AdjAng =  0;
  int AdjDist = 0;
  int OppDist = 0;
  double ObjHeight = 0;
  FastCenter();
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







bool Sweep() {
  if (stepperAngle != 0) {
    Serial2.print("\nSweeping for Objects\n");
    FastCenter();
  }
  LevelTOF();
  while (stepperAngle < 60 && ObjectDetection() == false) {
    StepperLeft(1);
    stepperAngle++;
    Serial2.print("\nStepper Angle:\t");
    Serial2.println(stepperAngle);
    delay(20);
  }
  if (ObjectDetection() == true)
  {
    return true;
  }
  StepperRight(stepperAngle);
  stepperAngle = 0;

  while (stepperAngle < 45 && ObjectDetection() == false) {
    StepperRight(1);
    stepperAngle++;
    Serial2.print("\nStepper Angle: \t");
    Serial2.println(stepperAngle);
  }
  if (ObjectDetection() == true)
  {
    return true;
  }
  FastCenter();
  return false;
}
