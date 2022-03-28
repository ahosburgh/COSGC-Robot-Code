void MoveForward(int dir) {

  Navigation(dir);
  DCForward();

  unsigned long currentTime = millis();
  unsigned long prevTime = millis();

  while (currentTime - prevTime < 5000 && ObjectDetection() == false && IMURoll() > -40 && IMURoll() < 40) {

    Serial2.print("Moving Forward Towards: \t");
    Serial2.print(dir);
    Serial2.print("\tDistance Measured: \t");
    Serial2.print(GetDistance());
    Serial2.print("\tStepper Angle: \t");
    Serial2.println(stepperAngle);

    currentTime = millis();
    LevelTOF();
    /////////////////////////////////////////////////////////// this whole big chunk moves the stepper motor in single motions so we can still do other stuff between each swing
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
    //////////////////////////////////////////////////////////////// end big chunk
  }
  DCStop();
  if (ObjectDetection() == true) {
    Serial2.print("\n\t OBJECT DETECTED AT: \t");
    Serial2.println(GetDistance());
    Serial2.println("");
    //MeasureObject();
    Avoidence();
  }
  if (IMURoll() < -35) {
    DCBack(3000);
    TurnRight(30);
  }
  if (IMURoll() > 35) {
    DCBack(3000);
    TurnLeft(30);
  }
  Navigation(dir);
}





//void Drift(int dir) {
//  int tempValue = 0;
//  if (dir < -170) {
//    tempValue = dir + 350;
//    if (IMUDirection() > dir + 10)
//    {
//      DCDriftRight();
//      Serial2.println("DCDriftRight");
//    }
//    else if (IMUDirection() < tempValue)
//    {
//      DCDriftLeft();
//      Serial2.println("DCDriftLeft");
//    }
//  }
//  else if (dir > 170) {
//    tempValue = dir - 350;
//    if (IMUDirection() < dir - 10)
//    {
//      DCDriftRight();
//      Serial2.println("DCDriftRight");
//    }
//    else if (IMUDirection() > tempValue)
//    {
//      DCDriftLeft();
//      Serial2.println("DCDriftLeft");
//    }
//  }
//  else
//  {
//    if (IMUDirection() < dir - 10)
//    {
//      DCDriftRight();
//      Serial2.println("DCDriftRight");
//    }
//    else (IMUDirection() > dir + 10);
//    {
//      DCDriftLeft();
//      Serial2.println("DCDriftLeft");
//    }
//  }
//}



//TURN LEFT
void TurnLeft(int deg)
{
  Serial2.println("\n======== TurnLeft Function Successfully Called ======== \n");        // Printing for testing

  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds

  unsigned long prevTime2 = millis();      // Assigning variable to keep track of the time passing in milliseconds

  int NumLed = 0;                         // Initalizing NumLed and setting it to 0 for blinker functions
  float startingDirection = 0;            // Creating local variable named startingDirection and setting it to 0
  float targetDirection = 0;              // Creating local variable named targetDirection and setting it to 0
  float tempValue = 0;                    // Creating variable named x for use in calculating targetDirection (holds a value while another variable is rewritten)

  startingDirection = IMUDirection();         // setting startingDirection to the current value returned from the function IMUDirection()
  targetDirection = startingDirection - deg;  // setting targetDirction = to the startingDirection - the deg value we passed to this function


  Serial2.print("\n Starting Direction: \t");     // Printing for testing
  Serial2.print(startingDirection);

  if (targetDirection < -180) {               // Calculating the target position if it goes over the -180 mark
    tempValue = -targetDirection - 180;
    targetDirection = 180 - tempValue;
  }

  Serial2.print("\t Target Direction: \t");       // Printing for testing
  Serial2.println(targetDirection);
  Serial2.println("\nBegin Turning Left\n");

  DCLeft();
  while (IMUDirection() > targetDirection + 10 || IMUDirection() < targetDirection - 10) {    // This while loop repeates untill the curentDirection has reached the target direction
    unsigned long currentTime = millis();
    //Left turn signal LED animation
    if (currentTime - prevTime > 150) {
      switch (NumLed) {
        case 0:
          digitalWrite(A2, HIGH);
          NumLed = 1;
          prevTime = currentTime;
          break;

        case 1:
          digitalWrite(A1, HIGH);
          NumLed = 2;
          prevTime = currentTime;
          break;

        case 2:
          digitalWrite(A0, HIGH);
          NumLed = 3;
          prevTime = currentTime;
          break;

        case 3:
          digitalWrite(A2, LOW);
          NumLed = 4;
          prevTime = currentTime;
          break;

        case 4:
          digitalWrite(A1, LOW);
          NumLed = 5;
          prevTime = currentTime;
          break;

        case 5:
          digitalWrite(A0, LOW);
          NumLed = 0;
          prevTime = currentTime;
          break;
      }
    }
    if (currentTime - prevTime2 > 8000) { // To get out if stuck
      Serial2.println("-----------Darwin Stuck-----------");
      DCRight();
      delay(2000);
      DCStop();
      DCBack(2000);
      prevTime2 = currentTime;
    }
    /////////////////////////////////////////////////////////// this whole big chunk moves the stepper motor in single motions so we can still do other stuff between each swing
    if (x == true) {
      if (stepperAngle < 10) {
        stepperAngle = stepperAngle + 10;
        StepperLeft(10);
      }
      else {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -10) {
        stepperAngle = stepperAngle - 10;
        StepperRight(10);
      }
      else {
        x = true;
      }
    }
    //////////////////////////////////////////////////////////////// end big chunk

    Serial2.print("Target Direction: \t");
    Serial2.print(targetDirection);
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
  }

  DCStop(); //Stops all DC motors
  LightsOut(); //Turns all lights off
  Serial2.println("\n Left Turn Complete \n");
  delay(MovementDelay); //Delay for set movement delay
}



//TURN RIGHT
void TurnRight(int deg)
{
  Serial2.println("\n======== TurnRight Function Successfully Called ======== \n");        // Printing for testing

  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  unsigned long prevTime2 = millis();      // Assigning variable to keep track of the time passing in milliseconds

  int NumLed = 0;                         // Initalizing NumLed and setting it to 0 for blinker functions
  float startingDirection = 0;            // Creating local variable named startingDirection and setting it to 0
  float targetDirection = 0;              // Creating local variable named targetDirection and setting it to 0
  float tempValue = 0;                    // Creating variable named x for use in calculating targetDirection (holds a value while another variable is rewritten)

  startingDirection = IMUDirection();         // setting startingDirection to the current value returned from the function IMUDirection()
  targetDirection = IMUDirection() + deg;  // setting targetDirction = to the startingDirection - the deg value we passed to this function


  Serial2.print("\n Starting Direction: \t");     // Printing for testing
  Serial2.print(startingDirection);

  if (targetDirection < -180) {               // Calculating the target position if it goes over the -180 mark
    tempValue = -targetDirection - 180;
    targetDirection = 180 - tempValue;
  }

  Serial2.print("\t Target Direction: \t");       // Printing for testing
  Serial2.println(targetDirection);
  Serial2.println("\nBegin Turning Right\n");

  DCRight();
  while (IMUDirection() > targetDirection + 10 || IMUDirection() < targetDirection - 10) {    // This while loop repeates untill the curentDirection has reached the target direction
    unsigned long currentTime = millis();
    //Right turn signal LED animation
    if (currentTime - prevTime > 150) {
      switch (NumLed) {
        case 0:
          digitalWrite(A3, HIGH);
          NumLed = 1;
          prevTime = currentTime;
          break;

        case 1:
          digitalWrite(A4, HIGH);
          NumLed = 2;
          prevTime = currentTime;
          break;

        case 2:
          digitalWrite(A5, HIGH);
          NumLed = 3;
          prevTime = currentTime;
          break;

        case 3:
          digitalWrite(A3, LOW);
          NumLed = 4;
          prevTime = currentTime;
          break;

        case 4:
          digitalWrite(A4, LOW);
          NumLed = 5;
          prevTime = currentTime;
          break;

        case 5:
          digitalWrite(A5, LOW);
          NumLed = 0;
          prevTime = currentTime;
          break;
      }
    }
    if (currentTime - prevTime2 > 8000) { // To get out if stuck
      Serial2.println("-----------Darwin Stuck-----------");
      DCLeft();
      delay(2000);
      DCStop();
      DCBack(2000);
      prevTime2 = currentTime;
    }
    /////////////////////////////////////////////////////////// this whole big chunk moves the stepper motor in single motions so we can still do other stuff between each swing
    if (x == true) {
      if (stepperAngle < 10) {
        stepperAngle = stepperAngle + 10;
        StepperLeft(10);
      }
      else {
        x = false;
      }
    }
    else
    {
      if (stepperAngle > -10) {
        stepperAngle = stepperAngle - 10;
        StepperRight(10);
      }
      else {
        x = true;
      }
    }
    //////////////////////////////////////////////////////////////// end big chunk

    Serial2.print("Target Direction: \t");
    Serial2.print(targetDirection);
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
  }
  DCStop(); //Stops all DC motors
  LightsOut(); //Turns all lights off
  Serial2.println("\n Right Turn Complete \n");
  delay(MovementDelay); //Delay for set movement delay
}
