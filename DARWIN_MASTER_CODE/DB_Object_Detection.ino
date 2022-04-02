// Uses the TOF sensor to measure a distance
int GetDistance() {
  int distance = 0;                                               // Creating and INT and naming it distance, to store the distance value recieved form the TOF sensor.

  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  distance = measure.RangeMilliMeter;

  return distance;                                                // Return the value stored in distance
}                                                                 // End of GetDistance()



// Uses the TOF Sensor to determine if an object is found
bool ObjectDetection() {

  int distance = 0;
  int minDistance = 480;

  distance = GetDistance();
  if (distance > minDistance) {
    Serial2.print("ObjectDetection() = FALSE \t Distance = \t");
    Serial2.print(distance);
    Serial2.print("\t");
    return false;
  }
  else
  {
    Serial2.print("ObjectDetection() = TRUE \t Distance = \t");
    Serial2.print(distance);
    Serial2.print("\t");
    return true;
  }
}




void MeasureObject() {

  double straightAng =  0;
  double straightDist = 0;
  double downAng = 0;
  double downDist = 0;
  double upAng = 0;
  double upDist = 0;
  double upperHeight = 0;
  double lowerHeight = 0;
  double height = 0;
  double leftAng = 0;
  double leftDist = 0;
  double leftWidth = 0;
  double rightAng = 0;
  double rightDist = 0;
  double rightWidth = 0;
  double width = 0;

  Serial2.print("LevelTOF();\n");
  LevelTOF(0);
  delay(500);

  straightDist = GetDistance();
  straightAng = ReadServoInDeg();
  if (straightDist > 900) {
    Serial2.print("\n\nstraightDist > 900 - Remeasuring\n\n");
    if (Sweep() == false) {
      return;
    }
  }
  Serial2.print("straightDist:\t");
  Serial2.print(straightDist);
  Serial2.print("\tstraightAng:\t");
  Serial2.println(straightAng);
  delay(500);

  // looking down
  downAng = (atan(235 / straightDist) * 180 / 3.14159265);
  downAng = downAng - straightAng;
  ServoPos(downAng);

  Serial2.print("\ndownAng:\t");
  Serial2.print(downAng);
  delay(500);

  downDist = GetDistance();

  if (downDist > 900) {
    Serial2.print("\n\ndownDist > 900 - Remeasuring\n\n");
    while (downDist > 900) {
      downDist = GetDistance();
      Serial2.print("\tdownDist:\t");
      Serial2.print(downDist);
    }
  }

  Serial2.print("\tdownDist:\t");
  Serial2.print(downDist);
  Serial2.print("\n\nServoPos(downAng):");
  Serial2.print(downAng);

  ServoPos(straightAng);
  Serial2.print("\nServoPos(straightAng):");
  Serial2.print(DegToServo(straightAng));
  delay(500);

  // Looking Up
  upAng = straightAng;
  upDist = GetDistance();
  Serial2.print("\n\nMeasuring Height\n\n");
  while (GetDistance() < 900 && upAng < 50) {
    upDist = GetDistance();
    upAng++;
    ServoPos(-upAng);
    Serial2.print("Distance:\t");
    Serial2.print(upDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(upAng);
    delay(100);
  }

  if (upDist > 900) {
    Serial2.print("\nOvershot!!! Reversing Direction\n");
    while (upDist > 900) {
      upDist = GetDistance();
      upAng--;
      ServoPos(-upAng);
      Serial2.print("Distance:\t");
      Serial2.print(upDist);
      Serial2.print("\tAngle:\t");
      Serial2.println(upAng);
      delay(100);
    }
  }

  Serial2.print("\nupDist:\t");
  Serial2.print(upDist);
  Serial2.print("\tupAng\t");
  Serial2.print(upAng);

  Serial2.print("\n\ndownDist:\t");
  Serial2.print(downDist);
  Serial2.print("\tstraightDist:\t");
  Serial2.print(straightDist);
  Serial2.print("\tupDist:\t");
  Serial2.print(upDist);
  Serial2.print("\ndownAng:\t");
  Serial2.print(downAng);
  Serial2.print("\tstraightAng\t");
  Serial2.print(straightAng);
  Serial2.print("\tupAng\t");
  Serial2.print(upAng);


  lowerHeight = ((downDist * sin(downAng * 180 / pi) + (straightDist * cos(downAng * 180 / pi)))) / 2;
  Serial2.print("\n\n\t\tLower Height in mm: \t");
  Serial2.println(lowerHeight);   // height in cm

  upperHeight = ((upDist * sin(upAng * 180 / pi) + (straightDist * cos(upAng * 180 / pi)))) / 2;
  Serial2.print("\n\n\t\tUpper Height in mm: \t");
  Serial2.println(upperHeight);   // height in cm

  height = abs(lowerHeight) + abs(upperHeight);
  Serial2.print("\n\n\t\tObject Height in mm: \t");
  Serial2.println(height);   // height in cm

  ServoPos(straightAng);
  Serial2.print("\nServoPos(straightAng):");
  Serial2.print(DegToServo(straightAng));
  delay(100);


  // measure left
  TOFStepper.setSpeed(600);
  Serial2.print("\n\nMeasuring Left Side\n\n");
  while (GetDistance() < 900) {
    leftDist = GetDistance();
    StepperLeft(10);
    leftAng = leftAng + 10;
    Serial2.print("Distance:\t");
    Serial2.print(leftDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(leftAng);
    delay(50);
  }
  while (GetDistance() > 900) {
    leftDist = GetDistance();
    StepperRight(20);
    leftAng = leftAng - 20;
    Serial2.print("Distance:\t");
    Serial2.print(leftDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(leftAng);
    delay(50);
  }
  TOFStepper.setSpeed(100);
  while (GetDistance() < 900) {
    leftDist = GetDistance();
    StepperLeft(1);
    leftAng = leftAng + 1;
    Serial2.print("Distance:\t");
    Serial2.print(leftDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(leftAng);
    delay(50);
  }

  if (leftDist > 900) {
    Serial2.print("\nOvershot!!! Reversing Direction\n");
    while (leftDist > 900) {
      leftDist = GetDistance();
      StepperRight(1);
      leftAng = leftAng - 1;
      Serial2.print("Distance:\t");
      Serial2.print(leftDist);
      Serial2.print("\tAngle:\t");
      Serial2.println(leftAng);
      delay(50);
    }
  }
  TOFStepper.setSpeed(600);
  StepperRight(leftAng);

  straightDist = GetDistance();
  Serial2.print("NEW straightDist:\t");
  Serial2.print(straightDist);
  delay(100);

  // measure right
  Serial2.print("\n\nMeasuring right Side\n\n");
  while (GetDistance() < 900) {
    rightDist = GetDistance();
    StepperRight(10);
    rightAng = rightAng + 10;
    Serial2.print("Distance:\t");
    Serial2.print(rightDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(rightAng);
    delay(50);
  }
  while (GetDistance() > 900) {
    rightDist = GetDistance();
    StepperLeft(20);
    rightAng = rightAng - 20;
    Serial2.print("Distance:\t");
    Serial2.print(rightDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(rightAng);
    delay(50);
  }
  TOFStepper.setSpeed(100);
  while (GetDistance() < 900) {
    rightDist = GetDistance();
    StepperRight(1);
    rightAng = rightAng + 1;
    Serial2.print("Distance:\t");
    Serial2.print(rightDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(rightAng);
    delay(50);
  }

  if (rightDist > 900) {
    Serial2.print("\nOvershot!!! Reversing Direction\n");
    while (rightDist > 900) {
      rightDist = GetDistance();
      StepperLeft(1);
      rightAng = rightAng - 1;
      Serial2.print("Distance:\t");
      Serial2.print(rightDist);
      Serial2.print("\tAngle:\t");
      Serial2.println(rightAng);
      delay(50);
    }
  }

  TOFStepper.setSpeed(600);
  StepperLeft(rightAng);
  Serial2.print("\n\nleftDist:\t");
  Serial2.print(leftDist);
  Serial2.print("\tstraightDist:\t");
  Serial2.print(straightDist);
  Serial2.print("\trightDist:\t");
  Serial2.print(rightDist);
  Serial2.print("\ndownAng:\t");
  Serial2.print(leftAng);
  Serial2.print("\tstraightAng\t");
  Serial2.print(straightAng);
  Serial2.print("\tupAng\t");
  Serial2.print(rightAng);

  leftWidth = leftDist * (sin(leftAng * 180 / pi));
  rightWidth = rightDist * (sin(rightAng * 180 / pi));
  width = abs(leftWidth)  + abs(rightWidth);

  Serial2.print("\n\n\t\t Object Width in cm: \t");
  Serial2.println(width / 10);   // height in cm


  objectWidth[arrayCounter] = width;
  objectHeight[arrayCounter] = height;
  arrayCounter = arrayCounter + 1;
  displayObjectArray();
  if (arrayCounter > 10) {
    arrayCounter = 0;
    Serial2.println("\tObject table full --- Creating new table");
    for (int i = 0; i < 10; i++) {
      objectWidth[i] = 0;
      objectHeight[i] = 0;
    }
  }

  delay(5000);

}



void displayObjectArray() {
  Serial2.println("\t---- Objects Detected ----");
  Serial2.print("Object #\t");
  Serial2.print("Width\t");
  Serial2.println("Height");

  for (int i = 0; i < 10; i++)
  {
    Serial2.print("\t#");
    Serial2.print(i);
    Serial2.print("\t");
    Serial2.print(objectWidth[i]);
    Serial2.print("\t");
    Serial2.println(objectHeight[i]);
  }
  delay(3000);
}


bool Sweep() {

  // Manually call a FastCenter() before running Sweep
  int distance = 0;
  stepperAngle = 0;
  TOFStepper.setSpeed(600);

  //Forward
  LevelTOF(18);
  distance = GetDistance();
  if (distance < 450) {
    Serial2.print("\nObject found at angle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tDistance:\t");
    Serial2.println(distance);
    DCStop();
    return true;
  }
  Serial2.print("Distance:\t");
  Serial2.print(distance);
  Serial2.print("\tStepper Angle:\t");
  Serial2.println(stepperAngle);

  // Left 65
  LevelTOF(50);
  stepperAngle = 55;
  LorR = false;
  StepperLeft(55);
  distance = GetDistance();
  if (distance < 274) {
    Serial2.print("\nObject found at angle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tDistance:\t");
    Serial2.println(distance);
    DCStop();
    return true;
  }
  Serial2.print("Distance:\t");
  Serial2.print(distance);
  Serial2.print("\tStepper Angle:\t");
  Serial2.println(stepperAngle);
  
  // Right 50
  LevelTOF(23);
  stepperAngle = 40;
  LorR = true;
  StepperRight(95);
  distance = GetDistance();
  if (distance < 522) {
    Serial2.print("\nObject found at angle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tDistance:\t");
    Serial2.println(distance);
    DCStop();
    return true;
  }
  Serial2.print("Distance:\t");
  Serial2.print(distance);
  Serial2.print("\tStepper Angle:\t");
  Serial2.println(stepperAngle);
  
  // Left 50
  LevelTOF(23);
  stepperAngle = 40;
  LorR = false;
  StepperLeft(80);
  distance = GetDistance();
  if (distance < 522) {
    Serial2.print("\nObject found at angle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tDistance:\t");
    Serial2.println(distance);
    DCStop();
    return true;
  }
  Serial2.print("Distance:\t");
  Serial2.print(distance);
  Serial2.print("\tStepper Angle:\t");
  Serial2.println(stepperAngle);
  
  // Right 65
  LevelTOF(23);
  stepperAngle = 55;
  LorR = true;
  StepperRight(95);
  distance = GetDistance();
  if (distance < 522) {
    Serial2.print("\nObject found at angle:\t");
    Serial2.print(stepperAngle);
    Serial2.print("\tDistance:\t");
    Serial2.println(distance);
    DCStop();
    return true;
  }
  Serial2.print("Distance:\t");
  Serial2.print(distance);
  Serial2.print("\tStepper Angle:\t");
  Serial2.println(stepperAngle);
  StepperLeft(45);
  
  return false;
  
}
