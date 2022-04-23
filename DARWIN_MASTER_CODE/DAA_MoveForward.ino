void MoveForward(int dir, int distInTime, int center) { //Function takes in a direction, an amount of time to move forward, and if you want to fast center(1) or not(0)
  Serial2.println("\n\t=========Start of Move Forward=========");
  if (dir == GoldenDirection) { //Moving towards golden direction
    Serial2.println("\t=========TOWARDS GOLDEN DIRECTION=========\n");
  }

  bool timeCheck = true; //Variable to update time
  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  unsigned long currentTime = millis();

  if (distInTime == 0) { //Continuously run function not for a set amount of time
    timeCheck = false;
    distInTime = 1;
    prevTime = 0;
    currentTime = 0;
  }

  if (center == 1) { //fast center
    FastCenter();
    delay(100);
  }

  bool sweep = Sweep();  //run sweep function and store result
  bool navigation = Navigation(dir); //run navigation function and store result
  bool roll = IMURoll(); //run roll function and store result
  unsigned long prevSensorTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  unsigned long currentSensorTime = millis();

  if (sweep == false && navigation == true && roll == true && currentTime - prevTime < distInTime) { //If no issues are present
    DCForward(); //moves rover forward
    while (sweep == false && navigation == true && roll == true && currentTime - prevTime < distInTime) { //while there are no issues
      DCForward();
      if (timeCheck == true) { //update time
        currentTime = millis();
      }
      currentSensorTime = millis(); //update currentSensorTime
      if (currentSensorTime - prevSensorTime > 10000) { //if 10 seconds has passed since last time prevSensorTime was updated
        Serial2.println("Re-centering sensor");
        DCStop(); //Stop the rover
        FastCenter(); //center the sensor
        delay(100);
        prevSensorTime = millis(); //update prevSensorTime
        DCForward(); //Go back to moving forward

      }
      Serial2.print("Moving Forward Towards: \t");
      Serial2.print(dir);
      Serial2.print("\t");
      sweep = MovingSweep(); //run sweep function
      navigation = Navigation(dir); //run navigation function
      roll = IMURoll(); //run roll function

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Move Forward Drift I put the drift inside the while because of reasons


      while (sweep == false && navigation == false && roll == true && currentTime - prevTime < distInTime) { //drift function(No longer moving towards golden direction)
        int switchCase = 0; //start switch case 0
        int tempValue = 0; //declare a new variable to store direction numbers
        float currentDirection = IMUDirection(); //update current direction
        Serial2.print("Drifting Towards: \t");
        Serial2.println(dir);
        currentDirection = IMUDirection(); //update current direction

        if (timeCheck == true) { //update time
          currentTime = millis();
        }
        currentSensorTime = millis(); //update currentSensorTime
        if (currentSensorTime - prevSensorTime > 5000) { //if 10 seconds has passed since last time prevSensorTime was updated
          Serial2.println("Re-centering sensor");
          DCStop(); //Stop the rover
          FastCenter(); //center the sensor
          delay(100);
          prevSensorTime = millis(); //update prevSensorTime
          DCForward(); //Go back to moving forward
        }
        
        if (dir < 10) { //determines which case to call depending on value of direction given
          switchCase = 0;
        }
        else if (dir > 350) {
          switchCase = 1;
        }
        else if (dir <= 350 && dir >= 10) {
          switchCase = 2;
        }

        switch (switchCase)
        {
          case 0:
            Serial2.println("\n Case 0 \n");
            tempValue = 360 - dir; //used to determine left side of direction
            if (currentDirection < dir + 10 || currentDirection > tempValue - 10) { //within window?
              Serial2.print("\tDrift Complete\n");
              DCForward();
            }
            else if (currentDirection > dir + 10 && currentDirection < dir + 180) { //too far right?
              Serial2.print("\t   Drifting Left   \n");
              DCDriftLeft();
            }
            else if (currentDirection < tempValue - 10 && currentDirection > tempValue - 180) { //too far left?
              Serial2.print("\t   Drifting Right   \n");
              DCDriftRight();
            }
            break;

          case 1:
            Serial2.println("\n Case 1 \n");
            tempValue = 360 - dir;
            if (currentDirection > dir - 10 || currentDirection < tempValue + 10) {
              Serial2.println("\tDrift Complete\n");
              DCForward();
            }
            else if (currentDirection < dir - 10 && currentDirection > dir - 180) {
              Serial2.print("\t   Drifting Right   \n");
              DCDriftRight();
            }
            else if (currentDirection > tempValue + 10 && currentDirection < 180 - tempValue) {
              Serial2.print("\t   Drifting Left   \n");
              DCDriftLeft();
            }
            break;


          case 2:
            Serial2.println("\n Case 3 \n");
            tempValue = 360 - dir;
            if (currentDirection > dir - 10 && currentDirection < dir + 10) {
              Serial2.println("\tDrift Complete\t");
              DCForward();
            }
            else if (currentDirection < dir - 10 || currentDirection > tempValue - 180) {
              Serial2.print("\t   Drifting Right   \n");
              DCDriftRight();
            }
            else if (currentDirection > tempValue + 10 && currentDirection < 180 - tempValue) {
              Serial2.print("\t   Drifting Left   \n");
              DCDriftLeft();
              
            }
            break;
        }
        sweep = MovingSweep(); //calls sweep function
        navigation = Navigation(dir); //calls navigation function
        roll = IMURoll(); //calls roll function
      }
    }
  }

  Serial2.println("\nEvent Detected, Exiting Loop\n");

  if (sweep == true) { //if an object is detected
    Serial2.println("\nsweep = true\n");
    Serial2.print("\tTrig Distance:\t");
    Serial2.println(GetDistance());

    Avoidence(); //calls avoidence function
    CenterRobot(); //centers robot
  }

  if (roll == false) { //robot is tilting past allowed value(will roll over if continuing to move forward)
    Serial2.println("\nroll = false\n");
    DCBack(body); //back up a bodylength
    TurnLeft(45); //turn left 45 degrees
    int dir = IMUDirection(); //store direction as current heading
    MoveForward(dir, body, 0); //move forward in the new direction for a bodylength of time and do not center
    TurnRight(45); //turn right 45 degrees
  }
  if (Navigation == false){
    CenterRobot();
  }
}
