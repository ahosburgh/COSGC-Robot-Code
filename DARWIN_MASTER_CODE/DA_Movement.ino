bool Navigation(float dir) {
  int tempValue = 0;
  float currentDirection = IMUDirection();
  float targetDirection = 0;

  Serial2.print("Navigation Check On: \t");
  Serial2.print(dir);
  int switchCase = 0;
  if (dir < 10) {
    switchCase = 0;
  }
  else if (dir > 350) {
    switchCase = 1;
  }
  else if (dir <= 350 && dir >= 10)
  {
    switchCase = 2;
  }
  switch (switchCase)
  {
    case 0:
      tempValue = 360 - dir;
      if (currentDirection < dir + 10 || currentDirection > tempValue - 10)
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

        //        tempValue = dir + 180;
        //        if (currentDirection > tempValue) {
        //          targetDirection = 180 - currentDirection + tempValue;
        //          Serial2.print("Turning Right \t") ;
        //          Serial2.println(targetDirection);
        //          TurnRight(targetDirection);
        //        }
        //        else if (currentDirection < tempValue) {
        //          targetDirection = -1 * (dir - currentDirection);
        //          Serial2.print("Turning Left \t") ;
        //          Serial2.println(targetDirection);
        //          TurnLeft(targetDirection);
        //        }
      }
      break;


    case 1:
      tempValue = 360 - dir;
      if (currentDirection > dir - 10 || currentDirection < tempValue)
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

        //        tempValue = dir - 180;
        //        if (currentDirection > tempValue) {
        //          targetDirection = 180 - currentDirection + tempValue;
        //          Serial2.print("Turning Right \t") ;
        //          Serial2.println(targetDirection);
        //          TurnRight(targetDirection);
        //        }
        //        else if (currentDirection < tempValue) {
        //          targetDirection = -1 * (dir - currentDirection);
        //          Serial2.print("Turning Left \t") ;
        //          Serial2.println(targetDirection);
        //          TurnLeft(targetDirection);
        //        }

      }


    case 2:
      if (currentDirection > dir - 10 && currentDirection < dir + 10)
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

        //        if (dir < 0) {
        //          tempValue = dir + 180;
        //        }
        //        if (dir > 0) {
        //          tempValue = dir - 180;
        //        }
        //        if (currentDirection > tempValue) {
        //          targetDirection = 180 - currentDirection + tempValue;
        //          Serial2.print("Turning Right \t") ;
        //          Serial2.println(targetDirection);
        //          TurnRight(targetDirection);
        //        }
        //        else if (currentDirection < tempValue) {
        //          targetDirection = -1 * (dir - currentDirection);
        //          Serial2.print("Turning Left \t") ;
        //          Serial2.println(targetDirection);
        //          TurnLeft(targetDirection);
      }
      break;
  }
}




void MoveForward(int dir, int distInTime) {
  int turnAng = 0;
  bool timeCheck = true;
  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  unsigned long currentTime = millis();
  if (distInTime == 0) {
    timeCheck == false;
  }

  FastCenter();
  delay(100);
  if (Sweep() == false && Navigation(dir) == true && IMURoll() == true) {  // if its actually safe to move forward, then move forward.
    
    //DCForward();
    while (Sweep() == false && Navigation(dir) == true && IMURoll() == true && currentTime - prevTime < distInTime) {
      if (timeCheck = true) {
        currentTime = millis();
      }
      
      Serial2.print("Moving Forward Towards: \t");
      Serial2.print(dir);
    }
  }

  if (Sweep() == false) {
    Avoidence();
  }
  if (Navigation(dir) == false) {
    // If we are within 45 degrees of front
    // drift

    // else turn the angle we need to
    
  }
  if (IMURoll() == false) {
    DCBack(body);
    TurnLeft(45);
    int dir = IMUDirection();
    MoveForward(dir, body);
    TurnRight(45);
  }
}






void Drift(int dir) {
  
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



//TURN Right
void TurnRight(int deg)
{
  Serial2.println("\n======== TurnLeft Function Successfully Called ======== \n");        // Printing for testing

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
  Serial2.println("\nBegin Turning Left\n");

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




void Avoidence() {
  int turnAng = 0;

  MeasureObject();
  if (leftWidth > rightWidth) {
    turnAng = (atan(straightDist / (leftWidth + 580))) * 180 / pi;
    DCBack(body);
    TurnRight(turnAng);
  }
  else {
    turnAng = (atan(straightDist / (rightWidth + 580))) * 180 / pi;
    DCBack(body);
    TurnRight(turnAng);
  }
  /////
  ///// COPY AND PASTE PART OF MOVE FORWARD BUT TIME IT
  /////



}
