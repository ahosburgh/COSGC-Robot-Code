//TURN LEFT
void TurnLeft(int deg)
{
  Serial.println("\n=====TurnLeft Function Successfully Called===== ");        // Printing for testing

  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  int NumLed = 0;                         // Initalizing NumLed and setting it to 0 for blinker functions
  float currentDirection = 0;             // Creating local variable named currentDirection and setting it to 0
  float startingDirection = 0;            // Creating local variable named startingDirection and setting it to 0
  float targetDirection = 0;              // Creating local variable named targetDirection and setting it to 0
  float tempValue = 0;                    // Creating variable named x for use in calculating targetDirection (holds a value while another variable is rewritten)

  startingDirection = IMUDirection();         // setting startingDirection to the current value returned from the function IMUDirection()
  targetDirection = startingDirection - deg;  // setting targetDirction = to the startingDirection - the deg value we passed to this function


  Serial.print("\n Starting Direction: ");     // Printing for testing
  Serial.println(startingDirection);

  if (targetDirection < -180) {               // Calculating the target position if it goes over the -180 mark
    tempValue = -targetDirection - 180;
    targetDirection = 180 - tempValue;
  }

  Serial.print(" Target Direction: ");       // Printing for testing
  Serial.println(targetDirection);

  Serial.println("Begin Turning Left");
  currentDirection = IMUDirection();          // currentDirection is being updated to the value returned by the IMUDirection function


  while (currentDirection > targetDirection + 10 || currentDirection < targetDirection - 10) {    // This while loop repeates untill the curentDirection has reached the target direction
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
    DCLeft(); //Sets all DC motors to turn left
    currentDirection = IMUDirection();
    Serial.print("Target Direction: ");
    Serial.print(targetDirection);
    Serial.print("Current Direction: ");
    Serial.println(currentDirection);

    if (currentTime - prevTime > 8000) { // To get out if stuck
      DCRight();
      delay(2000);
      DCStop();
      DCBack(2000);
    }

    if (currentDirection < targetDirection - 10 || currentDirection < targetDirection - 30) {  // Correction if the robot overshoots
      Serial.print("\n---Correction Needed---\n");
      int correction = targetDirection - currentDirection;
      TurnRight(correction);
    }
  }
  DCStop(); //Stops all DC motors
  LightsOut(); //Turns all lights off  
  Serial.println(" Left Turn Complete ");
  Serial.println("----------LeftTurn Function Complete----------");
  Serial.println(" ");
  delay(MovementDelay); //Delay for set movement delay
}


//TURN RIGHT
void TurnRight(int deg)
{
  Serial.println(" ");
  Serial.println("=====TurnRight Function Successfully Called===== ");

  unsigned long prevTime = millis();
  int NumLed = 0;
  float currentDirection = 0;
  float startingDirection = 0;
  float targetDirection = 0;


  startingDirection = IMUDirection();
  targetDirection = startingDirection + deg;

  Serial.print(" Starting Direction: ");
  Serial.println(startingDirection);

  if (targetDirection > 180) {     // Calculating the target position if it goes over the -180 mark
    deg = -targetDirection + 180;
    targetDirection = -180 - deg;
  }

  Serial.print("Target Direction: ");
  Serial.println(targetDirection);

  Serial.println("Begin Turning Right");
  currentDirection = IMUDirection();



  while (currentDirection > targetDirection + 10 || currentDirection < targetDirection - 10) {
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
    DCRight(); //Sets all DC motors to turn right
    currentDirection = IMUDirection();
    Serial.print("Target Direction: ");
    Serial.print(targetDirection);
    Serial.print("Current Direction: ");
    Serial.println(currentDirection);

    if (currentTime - prevTime > 8000) { // To get out if stuck
      DCLeft();
      delay(2000);
      DCStop();
      DCBack(2000);
    }

    if (currentDirection > targetDirection - 10 || currentDirection > targetDirection - 30) {  // Correction if the robot overshoots
      Serial.print("\n---Correction Needed---\n");
      int correction = targetDirection - currentDirection;
      TurnLeft(correction);
    } // End of if correction
  }   // End of While
  DCStop(); //Stops all DC Motors
  LightsOut(); //Turns all lights off
  Serial.println(" Right Turn Complete ");
  Serial.println("----------RightTurn Function Complete----------");
  Serial.println(" ");
  delay(MovementDelay); //Delay for set movement delay
}
