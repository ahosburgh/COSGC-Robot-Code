void MoveForward(int dir, int distInTime) {
  Serial2.println("Start of move forward");
  if (dir == GoldenDirection) {
    Serial2.println("TOWARDS GOLDEN DIRECTION");
  }

  int turnAng = 0;
  bool timeCheck = true;
  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  unsigned long currentTime = millis();

  if (distInTime == 0) {
    timeCheck = false;
    distInTime = 1;
    prevTime = 0;
    currentTime = 0;
  }

  FastCenter();
  delay(100);

  bool sweep = Sweep();
  bool navigation = Navigation(dir);
  bool roll = IMURoll();

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Move Forward Clean
  if (sweep == false && navigation == true && roll == true && currentTime - prevTime < distInTime) {
    //DCForward();
    while (sweep == false && navigation == true && roll == true && currentTime - prevTime < distInTime) {

      if (timeCheck == true) {
        currentTime = millis();
      }

      Serial2.print("Moving Forward Towards: \t");
      Serial2.print(dir);
      Serial2.print("\t");
      sweep = Sweep();
      navigation = Navigation(dir);
      roll = IMURoll();
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Move Forward Drift I put the drift inside the while because of reasons


      while (sweep == false && navigation == false && roll == true && currentTime - prevTime < distInTime) {
        int switchCase = 0;
        int tempValue = 0;
        float currentDirection = IMUDirection();
        float targetDirection = 0;
        Serial2.print("Drifting Towards: \t");
        Serial2.println(dir);
        currentDirection = IMUDirection();

        if (timeCheck == true) {
          currentTime = millis();
        }

        if (dir < 5) {
          switchCase = 0;
        }
        else if (dir > 355) {
          switchCase = 1;
        }
        else if (dir <= 355 && dir >= 5) {
          switchCase = 2;
        }

        switch (switchCase)
        {
          case 0:
          Serial2.println("\n Case 0 \n");
            tempValue = 360 - dir;
            if (currentDirection < dir + 5 || currentDirection > tempValue - 5) {
              Serial2.print("\tDrift Complete\n");
            }
            else {
              if (currentDirection > dir + 5 && currentDirection < dir + 180) {
                Serial2.print("\t   Drifting Left   \n");
              }
              else if (currentDirection < tempValue - 5 && currentDirection > tempValue - 180) {
                Serial2.print("\t   Drifting Right   \n");
              }
            }
            break;

          case 1:
          Serial2.println("\n Case 1 \n");
            tempValue = 360 - dir;
            if (currentDirection > dir - 5 || currentDirection < tempValue + 5) {
              Serial2.println("\tDrift Complete\n");
            }
            else {
              if (currentDirection < dir - 5 && currentDirection > dir - 180) {
                Serial2.print("\t   Drifting Right   \n");
              }
              else if (currentDirection > tempValue + 5 && currentDirection < 180 - tempValue) {
                Serial2.print("\t   Drifting Left   \n");
              }
            }


          case 2:
          Serial2.println("\n Case 3 \n");
            tempValue = 360 - dir;
            if (currentDirection > dir - 5 && currentDirection < dir + 5) {
              Serial2.println("\tDrift Complete\t");
            }
            else {
              if (currentDirection < dir - 5 || currentDirection > tempValue - 180) {
                Serial2.print("\t   Drifting Right   \n");
              }
              else if (currentDirection > tempValue + 5 && currentDirection < 180 - tempValue) {
                Serial2.print("\t   Drifting Left   \n");
              }
            }
            break;
        }
        sweep = Sweep();
        navigation = Navigation(dir);
        roll = IMURoll();
      }
    }
  }

  Serial2.println("\nEvent Detected, Exiting Loop\n");

  if (sweep == true) {
    if (leftAng > rightAng) {     // thanks to sweep, only one of these should be above 0 at any given time.
      TurnLeft(leftAng);
      Serial2.println("turn left to face object");
    }
    else {
      TurnRight(rightAng);
      Serial2.println("turn right to face object");
    }
    Avoidence();
    CenterRobot();
  }

  if (roll == false) {
    Serial2.println("IMURoll");

    //    DCBack(body);
    //    TurnLeft(45);
    //    int dir = IMUDirection();
    //    MoveForward(dir, body);
    //    TurnRight(45);
  }
}


bool Navigation(float dir) {
  int tempValue = 0;
  float currentDirection = IMUDirection();
  float targetDirection = 0;

  Serial2.print("Navigation Check On: \t");
  Serial2.print(dir);
  int switchCase = 0;
  if (dir < 5) {
    switchCase = 0;
  }
  else if (dir > 355) {
    switchCase = 1;
  }
  else if (dir <= 355 && dir >= 5)
  {
    switchCase = 2;
  }
  switch (switchCase)
  {
    case 0:
      tempValue = 360 - dir;
      if (currentDirection < dir + 5 || currentDirection > tempValue - 5)
      {
        Serial2.print("\tIMU reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
        return true;
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");
        return false;
      }
      break;

    case 1:
      tempValue = 360 - dir;
      if (currentDirection > dir - 5 || currentDirection < tempValue + 5)
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
        return true;
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");
        return false;
      }

    case 2:
      if (currentDirection > dir - 5 && currentDirection < dir + 5)
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
        return true;
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");
        return false;
      }
      break;
  }
}



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

  if (targetDirection < 0) {               // Calculating the target position if it goes over the -180 mark
    targetDirection = 360 + targetDirection;
  }

  Serial2.print("\t Target Direction: \t");       // Printing for testing
  Serial2.println(targetDirection);
  Serial2.println("\nBegin Turning Left\n");

  //DCLeft();
  while (Navigation(targetDirection) == false) {    // This while loop repeates untill the curentDirection has reached the target direction
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
    //    if (currentTime - prevTime2 > 8000) { // To get out if stuck
    //      Serial2.println("-----------Darwin Stuck-----------");
    //      DCRight();
    //      delay(2000);
    //      DCStop();
    //      DCBack(2000);
    //      prevTime2 = currentTime;
    //      DCLeft();
    //    }
    //    Serial2.print("Target Direction: \t");
    //    Serial2.print(targetDirection);
    //    Serial2.print("Current Direction: \t");
    //    Serial2.println(IMUDirection());
  }

  DCStop(); //Stops all DC motors
  LightsOut(); //Turns all lights off
  Serial2.println("\n Left Turn Complete \n");
  delay(MovementDelay); //Delay for set movement delay
}



//TURN Right
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
  targetDirection = startingDirection + deg;  // setting targetDirction = to the startingDirection - the deg value we passed to this function


  Serial2.print("\n Starting Direction: \t");     // Printing for testing
  Serial2.print(startingDirection);

  if (targetDirection > 360) {               // Calculating the target position if it goes over the -180 mark
    targetDirection = targetDirection - 360;
  }

  Serial2.print("\t Target Direction: \t");       // Printing for testing
  Serial2.println(targetDirection);
  Serial2.println("\nBegin Turning Right\n");

  //DCRight();
  while (Navigation(targetDirection) == false) {    // This while loop repeates untill the curentDirection has reached the target direction
    unsigned long currentTime = millis();
    //Right turn signal LED animation
    if (currentTime - prevTime > 150) {
      switch (NumLed) {
        case 0:
          digitalWrite(A2, LOW);
          NumLed = 1;
          prevTime = currentTime;
          break;

        case 1:
          digitalWrite(A1, LOW);
          NumLed = 2;
          prevTime = currentTime;
          break;

        case 2:
          digitalWrite(A0, LOW);
          NumLed = 3;
          prevTime = currentTime;
          break;

        case 3:
          digitalWrite(A2, HIGH);
          NumLed = 4;
          prevTime = currentTime;
          break;

        case 4:
          digitalWrite(A1, HIGH);
          NumLed = 5;
          prevTime = currentTime;
          break;

        case 5:
          digitalWrite(A0, HIGH);
          NumLed = 0;
          prevTime = currentTime;
          break;
      }
    }
    //    if (currentTime - prevTime2 > 8000) { // To get out if stuck
    //      Serial2.println("-----------Darwin Stuck-----------");
    //      DCRight();
    //      delay(2000);
    //      DCStop();
    //      DCBack(2000);
    //      prevTime2 = currentTime;
    //      DCLeft();
    //    }
    //    Serial2.print("\nTarget Direction: \t");
    //    Serial2.print(targetDirection);
    //    Serial2.print("Current Direction: \t");
    //    Serial2.println(IMUDirection());
  }

  DCStop(); //Stops all DC motors
  LightsOut(); //Turns all lights off
  Serial2.println("\n Right Turn Complete \n");
  delay(MovementDelay); //Delay for set movement delay
}




void Avoidence() {
  int turnAng = 0;
  int forward = body * 5;
  FastCenter();
  MeasureObject();
  if (leftWidth > rightWidth) {
    turnAng = (atan(straightDist / (leftWidth))) * 180 / pi;
    //DCBack(body);
    TurnRight(turnAng);
  }
  else {
    turnAng = (atan(straightDist / (rightWidth))) * 180 / pi;
    //DCBack(body);
    TurnRight(turnAng);
  }
  turnAng = IMUDirection();
  MoveForward(turnAng, forward);
}


void CenterRobot() {

  int tempValue = 0;
  float currentDirection = IMUDirection();
  float targetDirection = 0;

  Serial2.println("\nCentering Back to GoldenDirection\t");

  int switchCase = 0;

  if (GoldenDirection < 5) {
    switchCase = 0;
  }
  else if (GoldenDirection > 355) {
    switchCase = 1;
  }
  else if (GoldenDirection <= 355 && GoldenDirection >= 5)
  {
    switchCase = 2;
  }

  switch (switchCase)
  {
    case 0:
      tempValue = 360 - GoldenDirection;
      if (currentDirection < GoldenDirection + 5 || currentDirection > tempValue - 5)
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

        if (currentDirection > GoldenDirection + 5 && currentDirection < GoldenDirection + 180) {
          targetDirection = currentDirection - GoldenDirection;
          TurnLeft(targetDirection);
        }
        else if (currentDirection < tempValue - 5 && currentDirection > tempValue - 180) {
          targetDirection = tempValue - currentDirection;
          TurnRight(targetDirection);
        }
        else {
          Serial2.println(" UH OH CURRENT DIRECTION NOT WITHIN BOUNDS OF THE TURN LOGIC. FIX CODE");
          Serial2.print("TargetDirection: \t");
          Serial2.print(GoldenDirection);
          Serial2.print("\tCurrentDirection: \t");
          Serial2.print(currentDirection);
          while (1);
        }
      }
      break;

    case 1:
      tempValue = 360 - GoldenDirection;
      if (currentDirection > GoldenDirection - 5 || currentDirection < tempValue + 5)
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = TRUE\t");
        ;
      }
      else
      {
        Serial2.print("\tIMU Reading: \t") ;
        Serial2.print(currentDirection);
        Serial2.println("\tNavigation = FALSE\t");

        if (currentDirection < GoldenDirection - 5 && currentDirection > GoldenDirection - 180) {
          targetDirection = GoldenDirection - currentDirection;
          TurnRight(targetDirection);
        }
        else if (currentDirection > tempValue + 5 && currentDirection < 180 - tempValue) {
          targetDirection = currentDirection - tempValue;
          TurnLeft(targetDirection);
        }
        else {
          Serial2.println(" UH OH CURRENT DIRECTION NOT WITHIN BOUNDS OF THE TURN LOGIC. FIX CODE");
          Serial2.print("TargetDirection: \t");
          Serial2.print(GoldenDirection);
          Serial2.print("\tCurrentDirection: \t");
          Serial2.print(currentDirection);
          while (1);
        }
      }

    case 2:
      tempValue = 360 - GoldenDirection;
      if (currentDirection > GoldenDirection - 5 && currentDirection < GoldenDirection + 5)
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

        if (currentDirection < GoldenDirection - 5 || currentDirection > tempValue - 180) {/////////////////////////////////////////////////////////////////////////////////////
          targetDirection = GoldenDirection - currentDirection;
          TurnRight(targetDirection);
        }
        else if (currentDirection > tempValue + 5 && currentDirection < 180 - tempValue) {
          targetDirection = currentDirection - tempValue;
          TurnLeft(targetDirection);
        }
        else {
          Serial2.println(" UH OH CURRENT DIRECTION NOT WITHIN BOUNDS OF THE TURN LOGIC. FIX CODE");
          Serial2.print("TargetDirection: \t");
          Serial2.print(GoldenDirection);
          Serial2.print("\tCurrentDirection: \t");
          Serial2.print(currentDirection);
          while (1);
        }
      }
      break;
  }
}
