// Uses the TOF sensor to measure a distance
int GetDistance() {
  int distance = 0;                                                   // Creating and INT and naming it distance, to store the distance value recieved form the TOF sensor.

  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  distance = measure.RangeMilliMeter;

  return distance;                                                // Return the value stored in distance
}                                                                 // End of GetDistance()


// Uses the TOF Sensor to determine if an object is found
bool ObjectDetection() {

  int distance = 0;
  int minDistance = 600;

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
  int straightAng =  0;
  int straightDist = 0;
  int downAng = 0;
  int downDist = 0;
  int upAng = 0;
  int upDist = 0;
  int height = 0;
  int leftAng = 0;
  int leftDist = 0;
  int leftWidth = 0;
  int rightAng = 0;
  int rightDist = 0;
  int rightWidth = 0;
  int width = 0;

  FastCenter();
  LevelTOF();
  delay(500);
  straightDist = GetDistance();
  straightAng = ServoToDeg();
  // looking down
  downAng = (atan(235 / straightDist) * 180 / 3.14159265);
  ServoPos(-downAng);
  delay(500);
  downDist = GetDistance();
  ServoPos(straightAng);

  while (GetDistance() < 8000 || upAng < 50) {
    upDist = GetDistance();
    upAng++;
    ServoPos(upAng);
    delay(100);
  }

  if (upDist > 8000) {
    while (upDist > 8000) {
      upDist = GetDistance();
      upAng--;
      ServoPos(upAng);
      delay(100);
    }
  }
  height =  (sqrt(sq(downDist) - sq(straightDist))) + (sqrt(sq(upDist) - sq(straightDist)));
  Serial2.print(" Object Height in cm: \t");
  Serial2.println(height / 10);   // height in cm

  ServoPos(straightAng);
  delay(100);

  // measure left
  while (GetDistance() < 8000) {
    leftDist = GetDistance();
    StepperLeft(5);
    leftAng = leftAng + 5;
    delay(100);
  }
  if (leftDist > 8000) {
    while (leftDist > 8000) {
      leftDist = GetDistance();
      StepperRight(5);
      leftAng = leftAng - 5;
      delay(100);
    }
  }


  FastCenter();
  delay(100);
  straightDist = GetDistance();
  delay(100);

  // measure right
  while (GetDistance() < 8000) {
    rightDist = GetDistance();
    StepperRight(5);
    rightAng = rightAng + 5;
    delay(100);
  }
  if (leftDist > 8000) {
    while (leftDist > 8000) {
      rightDist = GetDistance();
      StepperLeft(5);
      rightAng = rightAng - 5;
      delay(100);
    }
  }

  leftWidth = leftDist * (sin(leftAng) * 180 / 3.14159265);
  rightWidth = rightDist * (sin(rightAng) * 180 / 3.14159265);
  width = leftWidth  + rightWidth;

  Serial2.print(" Object Width in cm: \t");
  Serial2.println(width / 10);   // height in cm

  if (arrayCounter = 11) {
    arrayCounter = 0;
  }
  objectWidth[arrayCounter] = width;
  objectHeight[arrayCounter] = height;
  arrayCounter++;
  displayObjectArray();
}



void displayObjectArray() {
  Serial2.println("\t---- Objects Detected ----");
  Serial2.print("Object #\t");
  Serial2.print("Width\t");
  Serial2.println("Height");

  for (int i = 0; i < 11; i++)
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
