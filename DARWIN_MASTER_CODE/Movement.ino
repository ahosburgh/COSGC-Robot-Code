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

  /*
  // Set speed of motors. Some are faster than others to stop turning malfunctions
  analogWrite(DCmotorFrontPWMA, med);   // Front Right
  analogWrite(DCmotorFrontPWMB, med);   // Front Left
  analogWrite(DCmotorMiddlePWMA, med);  // Middle Right
  analogWrite(DCmotorMiddlePWMB, slow);  // Middle Left
  analogWrite(DCmotorBackPWMA, slow);    // Back Right
  analogWrite(DCmotorBackPWMB, med);    // Back Left

  // All Right Motors Move Forward
  digitalWrite(DCmotorFrontAI1, LOW);   // Front Right Forwards
  digitalWrite(DCmotorFrontAI2, HIGH);
  digitalWrite(DCmotorMiddleAI1, LOW);  // Middle Right Forwards
  digitalWrite(DCmotorMiddleAI2, HIGH);
  digitalWrite(DCmotorBackAI1, LOW);    // Back Right Forwards
  digitalWrite(DCmotorBackAI2, HIGH);

  // All Left Motors Move Backwards
  digitalWrite(DCmotorFrontBI1, HIGH);       // Front Left Backwards
  digitalWrite(DCmotorFrontBI2, LOW);
  digitalWrite(DCmotorMiddleBI1, HIGH);      // Middle Left Backwards
  digitalWrite(DCmotorMiddleBI2, LOW);
  digitalWrite(DCmotorBackBI1, HIGH);        // Back Left Backwards
  digitalWrite(DCmotorBackBI2, LOW);
  */

  currentDirection = IMUDirection();
  Serial.print("Target Direction: ");
  Serial.print(targetDirection);
  Serial.print("Current Direction: ");
  Serial.println(currentDirection);

    if(currentDirection < targetDirection - 10 || currentDirection < targetDirection - 30){    // Correction if the robot overshoots 
      Serial.print("\n---Correction Needed---\n");
      int correction = targetDirection - currentDirection; 
      TurnRight(correction);
    }
  }
  DCStop();
  LightsOut();
  Serial.println(" Left Turn Complete ");
  Serial.println("----------LeftTurn Function Complete----------");
  Serial.println(" ");
  delay(MovementDelay);
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
    /*
    // Set speed of motors. Some are faster than others to stop turning malfunctions
    analogWrite(DCmotorFrontPWMA, med);   // Front Right
    analogWrite(DCmotorFrontPWMB, med);   // Front Left
    analogWrite(DCmotorMiddlePWMA, slow);  // Middle Right
    analogWrite(DCmotorMiddlePWMB, med);  // Middle Left
    analogWrite(DCmotorBackPWMA, med);    // Back Right
    analogWrite(DCmotorBackPWMB, slow);    // Back Left
    
    // All Right Motors Move Backwards
    digitalWrite(DCmotorFrontAI1, HIGH);   // Front Right Backwards
    digitalWrite(DCmotorFrontAI2, LOW);
    digitalWrite(DCmotorMiddleAI1, HIGH);  // Middle Right Backwards
    digitalWrite(DCmotorMiddleAI2, LOW);
    digitalWrite(DCmotorBackAI1, HIGH);    // Back Right Backwards
    digitalWrite(DCmotorBackAI2, LOW);
    
    // All Left Motors Move Forward
    digitalWrite(DCmotorFrontBI1, LOW);       // Front Left Forward
    digitalWrite(DCmotorFrontBI2, HIGH);
    digitalWrite(DCmotorMiddleBI1, LOW);      // Middle Left Forward
    digitalWrite(DCmotorMiddleBI2, HIGH);
    digitalWrite(DCmotorBackBI1, LOW);        // Back Left Forward
    digitalWrite(DCmotorBackBI2, HIGH);
    */

    currentDirection = IMUDirection();
    Serial.print("Target Direction: ");
    Serial.print(targetDirection);
    Serial.print("Current Direction: ");
    Serial.println(currentDirection);
    

      if(currentDirection > targetDirection - 10 || currentDirection > targetDirection - 30){    // Correction if the robot overshoots 
        Serial.print("\n---Correction Needed---\n");
        int correction = targetDirection - currentDirection; 
        TurnLeft(correction);
      } // End of if correction
    }   // End of While 
  DCStop();
  LightsOut();
  Serial.println(" Right Turn Complete ");
  Serial.println("----------RightTurn Function Complete----------");
  Serial.println(" ");
  delay(MovementDelay);
}
