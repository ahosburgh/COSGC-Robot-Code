void Animation() {
  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds
  unsigned long currentTime = millis();
  int wait = 0;

  LevelTOF(0);
  while (GetDistance() > 900) {
    Serial2.print("\nSearching for Object\n");
    wait = random(3000, 9000);
    currentTime = millis();
    if (currentTime - prevTime > wait) {

      Serial2.print("\ntimes up\n");
      Serial2.print(leftAng);
      Serial2.print("\t");
      Serial2.print(rightAng);

      if (rightAng > leftAng) {
        stepperCounter = random(5, 20);
        Serial2.print("\nlooking left ");
        Serial2.print(rightAng + stepperCounter);
        StepperLeft(rightAng);
        StepperLeft(stepperCounter);
        LevelTOF(random(20));
        leftAng = stepperCounter;
        prevTime = millis();
        currentTime = millis();
        rightAng = 0;
      }
      else if (leftAng >= rightAng) {
        stepperCounter = random(5, 20);
        Serial2.print("\nlooking right ");
        Serial2.print(leftAng + stepperCounter);
        StepperRight(leftAng);
        StepperRight(stepperCounter);
        LevelTOF(random(20));
        rightAng = stepperCounter;
        prevTime = millis();
        currentTime = millis();
        leftAng = 0;
      }
    }
  }

  int upAng = 0;

  while (GetDistance() <= 900 && upAng > -50) {
    upAng--;
    ServoPos(upAng);
  }

  while (GetDistance() >= 800 && upAng < 0){
    upAng++;
    ServoPos(upAng);
  }

  while (GetDistance() <= 800) {
    Serial2.print("\nLook at face");
    wait = random(2000, 4000);
    currentTime = millis();
    if (currentTime - prevTime > wait) {

      Serial2.print("\n look at face times up\n");
      Serial2.print(leftAng);
      Serial2.print("\t");
      Serial2.print(rightAng);

      if (rightAng > leftAng) {
        stepperCounter = random(5);
        Serial2.print("\nlooking left ");
        Serial2.print(rightAng + stepperCounter);
        StepperLeft(rightAng);
        StepperLeft(stepperCounter);
        leftAng = stepperCounter;
        prevTime = millis();
        currentTime = millis();
        rightAng = 0;
      }
      else if (leftAng >= rightAng) {
        stepperCounter = random(5);
        Serial2.print("\nlooking right ");
        Serial2.print(leftAng + stepperCounter);
        StepperRight(leftAng);
        StepperRight(stepperCounter);
        rightAng = stepperCounter;
        prevTime = millis();
        currentTime = millis();
        leftAng = 0;
      }
    }
  }
  Serial2.println("delay");
  delay(500);
}
