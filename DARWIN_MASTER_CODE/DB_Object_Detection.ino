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
  double straightHeight = 0;
  double downAng = 0;
  double downDist = 0;
  double downHeight = 0;
  double upAng = 0;
  double upDist = 0;
  double upHeight = 0;
  double height = 0;
  double leftAng = 0;
  double leftDist = 0;
  double leftWidth = 0;
  double rightAng = 0;
  double rightDist = 0;
  double rightWidth = 0;
  double width = 0;
  double slopeDist1 = 0;
  double slopeDist2 = 0;
  double slopeDist3 = 0;
  double slopeHeight1 = 0;
  double slopeHeight2 = 0;
  double slopeAng = 0;
  double slope = 0;
  bool small = false;                                                           // is the object small? are we running the small version of the code? true = yes;

  straightDist = GetDistance();                                                 // Measure the "straight" distance from the TOF sensor at a 0 degree angle
  straightAng = ReadServoInDeg();                                               // Measure the servos actual degree position and set it = to straight angle


  // SMALL MEASURE HEIGHT
  if (straightDist > 900) {                                                     // If the sensor cannot detect anything
    Serial2.print("\n\nMeasuring Height SMALL SMALL SMALL\n\n");                // Run the "Small Object" version of the height algrothim.
    small = true;                                                               // Set the bool upOrDown = to true. This means that we are looking at a "small" object < the servos 0 degree height
    upAng = straightAng;                                                        // upAng will count down from straightAng, but we need set them = first so we have something to subtract from.
    upDist = straightDist;                                                      // Also taking the value from straightDist and plugging it into upDist instead of remeasuring

    while (upDist > 900 && upAng < 50) {                                        // While upDist continues to measure "infinity", and so long as upAng doesnt go above 50 (limits of the servos range of motion)
      upAng++;                                                                  // add 1 to the upAng value
      upDist = GetDistance();                                                   // get a new measurement and set it = to upDist
      ServoPos(upAng);                                                          // set the servo to the new upAng position
      Serial2.print("Distance:\t");                                             // Printing for debugging
      Serial2.print(upDist);
      Serial2.print("\tAngle:\t");
      Serial2.println(upAng);
      delay(100);                                                               // wait for movement to complete
    }                                                                           // At this point we should have a intersected with the top of the object being measured. upDist = some value, and we can end the while loop

    straightDist = abs(upDist * cos(upAng));                                    // getting the value of straightDist (the flat line above upDist) using trig
    Serial2.print("\nupDist:\t");                                               // Printing for debugging
    Serial2.print(upDist);
    Serial2.print("\tupAng\t");
    Serial2.print(upAng);
    Serial2.print("\tstraightDist:\t");
    Serial2.println(straightDist);

    downAng = abs(atan (235 / straightDist) * 180 / pi);                        // downAng is the estimated angle to look at the bottom of the object. Use trig and the known value of the height of the sensor.
    Serial2.print("\ndownAng:\t");                                              // Printing for debugging
    Serial2.print(downAng);

    straightHeight = abs(straightDist * cos(downAng));                          // Calculating the big height between the estimated bottom of the object and the straightDist beam for future use.
    Serial2.print("\nstraightHeight:\t");                                       // Printing for debugging
    Serial2.print(straightHeight);

    upHeight = abs(upDist * sin(upAng));                                        // Calculating the estimated height between the top of the object and the straightDist beam for future use.
    Serial2.print("\nupHeight:\t");                                             // Printing for debugging
    Serial2.print(upHeight);

    ServoPos(downAng - straightAng);                                            // subtract straightAng from downAng to get the actual value to set the servo to.
    delay(100);                                                                 // wait for movement to complete

    downDist = GetDistance();                                                   // Get a distance value for downDist. This will be a point near the bottom of the object.

    if (downDist > 900) {                                                       // if downDist reads infinity
      Serial2.print("\n\ndownDist > 900 - Remeasuring\n\n");                    // Printing for debugging
      int i = 0;                                                                // Creating a quick counter
      while (downDist > 900 && i < 5) {                                         // Read value 5 more times just to be sure it's not a hardware error
        downDist = GetDistance();                                               // Get a new value for downDist
        Serial2.print("\tdownDist:\t");                                         // Printing for debugging
        Serial2.print(downDist);
        i++;                                                                    // Increase the counter by 1
        delay(500);                                                             // Wait to see if something changes in the real world,
      }                                                                         // End of while loop
    }                                                                           // End of if statement

    if (downDist > 900) {                                                       // If we do all of that and downDist is STILL infinity
      Serial2.print("\n\nPotential overhang discovered\n\n");                   // We might be looking at a hole or an overhang
      downDist =  abs(straightHeight * sin(downAng));                           // Set downDist by using the straightHeight value as the opposite side and the angle the servo is at to approximate a downwards distance to the bottom of the object.
    }                                                                           // End of if statement

    Serial2.print("\tdownDist:\t");                                             // Printing for debugging
    Serial2.print(downDist);
    Serial2.print("\n\nServoPos(downAng):");
    Serial2.print(downAng);

    downHeight = abs(downDist * sin(downAng));                                  // We need this value later for calculating angle. It is the height from the point the downDist beam made contact to the straightDist beam.
    Serial2.print("\tdownHeight\t");
    Serial2.print(downHeight);

    Serial2.print("\n\n\t\tup Height in mm: \t");
    Serial2.println(upHeight);   // height in cm

    Serial2.print("\n\n\t\tdown Height in mm: \t");
    Serial2.println(downHeight);   // height in cm

    height = abs(straightHeight - upHeight);                                         // Calculating Height by subtracting the smaller height calculated between the beam and the top of the object, from the large straightHeight value that goes all the way down to the bottom/
    Serial2.print("\n\n\t\tHeight in mm: \t");
    Serial2.println(height);   // height in cm
  }
  else                                                          // BIG MEASURE HEIGHT
  {

    Serial2.print("straightDist:\t");                                           // Printing for debugging
    Serial2.print(straightDist);
    Serial2.print("\tstraightAng:\t");
    Serial2.println(straightAng);

    downAng = abs(atan (235 / straightDist) * 180 / pi);                        // downAng is the estimated angle to look at the bottom of the object. Use trig and the known value of the height of the sensor.
    Serial2.print("\ndownAng:\t");                                              // Printing for debugging
    Serial2.print(downAng);

    ServoPos(downAng - straightAng);                                            // subtract straightAng from downAng to get the actual value to set the servo to.
    delay(100);                                                                 // wait for movement to complete);

    downDist = GetDistance();                                                   // Get a distance value for downDist. This will be a point near the bottom of the object.

    if (downDist > 900) {                                                       // if downDist reads infinity
      Serial2.print("\n\ndownDist > 900 - Remeasuring\n\n");                    // Printing for debugging
      int i = 0;                                                                // Creating a quick counter
      while (downDist > 900 && i < 5) {                                         // Read value 5 more times just to be sure it's not a hardware error
        downDist = GetDistance();                                               // Get a new value for downDist
        Serial2.print("\tdownDist:\t");                                         // Printing for debugging
        Serial2.print(downDist);
        i++;                                                                    // Increase the counter by 1
        delay(500);                                                             // Wait to see if something changes in the real world,
      }                                                                         // End of while loop
    }                                                                           // End of if statement

    straightHeight = abs(straightDist * cos(downAng));                          // Calculating the big height between the estimated bottom of the object and the straightDist beam for future use. This is also the estimated hight of the lower section of the object
    Serial2.print("\nstraightHeight:\t");                                       // Printing for debugging
    Serial2.print(straightHeight);

    if (downDist > 900) {                                                       // If we do all of that and downDist is STILL infinity
      Serial2.print("\n\nPotential overhang discovered\n\n");                   // We might be looking at a hole or an overhang
      downDist =  abs(straightHeight * sin(downAng));                                // Set downDist by using the straightHeight value as the opposite side and the angle the servo is at to approximate a downwards distance to the bottom of the object.
    }                                                                           // End of if statement

    Serial2.print("\tdownDist:\t");                                             // Printing for debugging
    Serial2.print(downDist);
    Serial2.print("\n\nServoPos(downAng):");
    Serial2.print(downAng);

    ServoPos(straightAng);                                                      // Setting servo back to its 0 degree point.
    Serial2.print("\nServoPos(straightAng):");                                  // Printing for debugging
    Serial2.print(DegToServo(straightAng));
    delay(100);                                                                 // Waiting for movement to complete

    upAng = straightAng;                                                        // Setting upAng = straightAng so we can start from the same point
    upDist = GetDistance();                                                     // Read the distance for upDist;
    Serial2.print("\n\nMeasuring Height\n\n");
    while (upDist < 900 && upAng < 50) {                                        // while loop to step the motor up until the top of the object is seen, or the angle of the servo is reached
      upDist = GetDistance();                                                   // Read distance
      upAng++;                                                                  // add 1 to the upAng value
      ServoPos(-upAng);                                                         // Move the servo to the - of upAng (to look up)
      Serial2.print("Distance:\t");                                             // Printing for debugging
      Serial2.print(upDist);
      Serial2.print("\tAngle:\t");
      Serial2.println(upAng);
      delay(100);                                                               // Wait for movement to complete
    }

    if (upDist > 900) {                                                         // upDist should be infinity. It's ok if its not, but if it is, we need to reverse direction to make contact with the top edge of the object again
      Serial2.print("\nEdge Detected - Reversing Direction\n");
      while (upDist > 900) {                                                    // while loop that is the same of the one just above, with opposite direction
        upDist = GetDistance();                                                 // Read distance
        upAng--;                                                                // subtract 1 from upAng value
        ServoPos(-upAng);                                                       // Move the servo to the - of the upAng (to look up)
        Serial2.print("Distance:\t");                                           // Printing for debugging
        Serial2.print(upDist);
        Serial2.print("\tAngle:\t");
        Serial2.println(upAng);
        delay(100);                                                             // Wait for movement to complete
      }
    }

    Serial2.print("\nupDist:\t");                                               // Printing for debugging
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

    downHeight = abs(downDist * sin(downAng));                                  // We need this value later for calculating angle. It is the height from the point the downDist beam made contact to the straightDist beam.
    Serial2.print("\tdownHeight\t");
    Serial2.print(downHeight);


    upHeight = abs(upDist * sin(upAng));                                         // upHeight is the height of the upper portion of the object
    Serial2.print("\n\n\t\tup Height in mm: \t");
    Serial2.println(upHeight);   // height in cm

    height = abs(straightHeight + upHeight);                                    // take the estimated lower height and add it to the measured upper height.
    Serial2.print("\n\n\t\tObject Height in mm: \t");
    Serial2.println(height);   // height in cm



    ServoPos(straightAng);                                                      // move servo back to origin
    Serial2.print("\nServoPos(straightAng):");
    Serial2.print(DegToServo(straightAng));
    delay(100);




  }                                                           // END OF MEASURE HEIGHT


  // MEASURE LEFT
  TOFStepper.setSpeed(600);                                                     // Set the stepper speed to the fastest it can handle
  Serial2.print("\n\nMeasuring Left Side\n\n");
  while (leftDist < 900 && leftAng < 100) {                                     // while the distance is not reading infinity, and the stepper hasn't moved more than 100 degrees
    leftDist = GetDistance();                                                   // Start setting the value for leftDist
    StepperLeft(10);                                                            // Move the stepper left 10 steps at a time (we are basically running towards the edge)
    leftAng = leftAng + 10;                                                     // increase the value of our leftAng value by 10
    Serial2.print("Distance:\t");                                               // Printing for debugging
    Serial2.print(leftDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(leftAng);
    delay(50);                                                                  // Wait just a tad for vibration to settle
  }                                                                             // End while

  while (leftDist > 900 && leftAng < 0) {                                       // The edge or limit has been detected. reverse direction until we see it again.
    leftDist = GetDistance();
    StepperRight(20);
    leftAng = leftAng - 20;
    Serial2.print("Distance:\t");
    Serial2.print(leftDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(leftAng);
    delay(50);
  }

  TOFStepper.setSpeed(100);                                                     // Set the stepper to a very low speed
  while (GetDistance() < 900 && leftAng < 100) {                                // slowly walk ourselves to the edge again.
    leftDist = GetDistance();
    StepperLeft(1);
    leftAng = leftAng + 1;
    Serial2.print("Distance:\t");
    Serial2.print(leftDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(leftAng);
    delay(50);
  }

  if (leftDist > 900) {                                                         // and walk it back slowly after the edge has been found again
    Serial2.print("\nEdge Found - Reversing Direction\n");
    while (leftDist > 900 && leftAng > 0) {
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

  TOFStepper.setSpeed(600);                                                     // Set stepper back to fastest speed
  StepperRight(leftAng);                                                        // Set it back to 0 by moving in the opposite direction the amount leftAng said it turned

  // MEASURE RIGHT
  rightAng = 0;                                                                 // Same thing just one the other side.
  Serial2.print("\n\nMeasuring right Side\n\n");
  while (GetDistance() < 900 && rightAng < 100) {
    rightDist = GetDistance();
    StepperRight(10);
    rightAng = rightAng + 10;
    Serial2.print("Distance:\t");
    Serial2.print(rightDist);
    Serial2.print("\tAngle:\t");
    Serial2.println(rightAng);
    delay(50);
  }
  while (GetDistance() > 900 && rightAng > 0) {                          //
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
  while (GetDistance() < 900 && rightAng < 100) {                          //
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
    Serial2.print("\nOvershot!!! Reversing Direction\n");                 //
    while (rightDist > 900 && rightAng > 0) {
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


  TOFStepper.setSpeed(600);                                               // Seting stepper back to fast speed
  StepperLeft(rightAng);
  FastCenter();                                                           // Centering the stepper since we are done with it

  Serial2.print("\n\nleftDist:\t");                                       // Printing for debugging
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

  leftWidth = abs(leftDist * sin(leftAng));                              // Use Trig to figure out the widths of each half of the object.
  rightWidth = abs(rightDist * sin(rightAng));

  width = leftWidth + rightWidth;                              // The width is = to the sum of the left and right width.

  Serial2.print("\n\n\t\t Object Width in cm: \t");                       // Printing for debugging
  Serial2.println(width / 10);                                            // Width in cm

  Serial2.print("\n\tCalculating slope of object\n");
  if (small == true) {                                                    // Calculating small object slope
    slopeAng = downAng - upAng;                                           // Calculate angle of one corner of the irregular triangle

  }
  else {                                                                  // Calculating large object slope
    slopeAng = downAng + upAng;                                           // Calculate angle of one corner of the irregular triangle
  }
  Serial2.print("\nslopeAng\t");
  Serial2.print(slopeAng);
  slopeHeight1 = abs(downDist * sin(slopeAng));                           // After getting the angle of the little corner, we can get the imaginary Height of the side that two invisible right triangles share
  Serial2.print("\nslopeHeight1\t");
  Serial2.print(slopeHeight1);
  slopeDist1 = abs(downDist * cos(slopeAng));                             // We also need the imaginary "adjacent" side of this floating right triangle, because this side is = a partial distance of upDist
  Serial2.print("\nslopeDist1\t");
  Serial2.print(slopeDist1);
  slopeDist2 = upDist - slopeDist1;                                       // See?
  Serial2.print("\nslopeDist2\t");
  Serial2.print(slopeDist2);
  slopeDist3 = sqrt(sq(slopeDist2) + sq(slopeHeight1));                   // Now we can calculate the final side of the second imaginary right triangle to complete it. This is also the actual length of the slope itself.
  Serial2.print("\nslopeDist3\t");
  Serial2.print(slopeDist3);

  if (small == true) {                                                // Calculating small object slope
    slopeHeight2 = abs(downHeight - upHeight);                                 // We now have the opposite side of a 3rd right triangle whos hypotenuse is actually the slope itself, and whos adjacent side is paralell with our 0 degree line.
  }
  else {                                                              // Calculating large object slope
    slopeHeight2 = abs(downHeight + upHeight);                                 // We didn't use downHeight for the actual height measurements, but it is usefull here for creating the final right triangle
  }
  Serial2.print("\nslopeHeight2\t");
  Serial2.print(slopeHeight2);


  slope = asin(slopeHeight2 / slopeDist3) * 180 / pi;                     // And this is the angle between the measured "top" and the measured "bottom" points      ang = arcsin h/d (but apperantly not cause arduino poopy)

  if (isnan(slope)) {
    Serial2.print("\nNAN SLOPE\n");
    slope = asin(slopeDist3 / slopeHeight2) * 180 / pi;
//    slope = abs(90 - slope);
  }
  Serial2.print("\nslope\t");
  Serial2.print(slope);

  objectWidth[arrayCounter] = width;                                      // Load everything into their arrays for viewing later
  objectHeight[arrayCounter] = height;
  objectAngle[arrayCounter] = slope;

  arrayCounter = arrayCounter + 1;                                        // increase the counter of the array
  displayObjectArray();                                                   // display the arrays current values

  if (arrayCounter = 10) {                                                // Check the cycle of the array and make a new one if necessary
    arrayCounter = 0;
    Serial2.println("\t--- Object table full --- \n\t--- Creating new table---");
    for (int i = 0; i < 10; i++) {                                        // It doesn't really make a "new" one. Just sets the old one to zero. But we're saving data seperatly so it doesn't matter.
      objectWidth[i] = 0;
      objectHeight[i] = 0;
      objectAngle[i] = 0;
    }
  }
  delay(5000);
}



void displayObjectArray() {
  Serial2.println("\n\t---- Objects Detected ----");
  Serial2.print("Object #\t");
  Serial2.print("Width\t");
  Serial2.print("Height\t");
  Serial2.println("Angle\t");

  for (int i = 0; i < 10; i++)
  {
    Serial2.print("\t#");
    Serial2.print(i);
    Serial2.print("\t");
    Serial2.print(objectWidth[i]);
    Serial2.print("\t");
    Serial2.print(objectHeight[i]);
    Serial2.print("\t");
    Serial2.println(objectAngle[i]);
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
