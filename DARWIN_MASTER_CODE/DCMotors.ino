void DCDriftLeft()
{
  // Set speed of motors
  analogWrite(DCmotorFrontPWMA, med);   // Front Right
  analogWrite(DCmotorFrontPWMB, slow);   // Front Left
  analogWrite(DCmotorMiddlePWMA, med);  // Middle Right
  analogWrite(DCmotorMiddlePWMB, slow);  // Middle Left
  analogWrite(DCmotorBackPWMA, med);    // Back Right
  analogWrite(DCmotorBackPWMB, slow);    // Back Left

  // All Right Motors move forward
  digitalWrite(DCmotorFrontAI1, LOW);       // Front Right Forward
  digitalWrite(DCmotorFrontAI2, HIGH);
  digitalWrite(DCmotorMiddleAI1, LOW);      // Middle Right Forward
  digitalWrite(DCmotorMiddleAI2, HIGH);
  digitalWrite(DCmotorBackAI1, LOW);        // Back Right Forward
  digitalWrite(DCmotorBackAI2, HIGH);

  // All Left Motors move forward
  digitalWrite(DCmotorFrontBI1, LOW);       // Front Left Forward
  digitalWrite(DCmotorFrontBI2, HIGH);
  digitalWrite(DCmotorMiddleBI1, LOW);      // Middle Left Forward
  digitalWrite(DCmotorMiddleBI2, HIGH);
  digitalWrite(DCmotorBackBI1, LOW);        // Back Left Forward
  digitalWrite(DCmotorBackBI2, HIGH);
}



void DCDriftRight() {

  // Set speed of motors.
  analogWrite(DCmotorFrontPWMA, slow);   // Front Right
  analogWrite(DCmotorFrontPWMB, med);   // Front Left
  analogWrite(DCmotorMiddlePWMA, slow);  // Middle Right
  analogWrite(DCmotorMiddlePWMB, med);  // Middle Left
  analogWrite(DCmotorBackPWMA, slow);    // Back Right
  analogWrite(DCmotorBackPWMB, med);    // Back Left

  // All Right Motors move forward
  digitalWrite(DCmotorFrontAI1, LOW);       // Front Right Forward
  digitalWrite(DCmotorFrontAI2, HIGH);
  digitalWrite(DCmotorMiddleAI1, LOW);      // Middle Right Forward
  digitalWrite(DCmotorMiddleAI2, HIGH);
  digitalWrite(DCmotorBackAI1, LOW);        // Back Right Forward
  digitalWrite(DCmotorBackAI2, HIGH);

  // All Left Motors move forward
  digitalWrite(DCmotorFrontBI1, LOW);       // Front Left Forward
  digitalWrite(DCmotorFrontBI2, HIGH);
  digitalWrite(DCmotorMiddleBI1, LOW);      // Middle Left Forward
  digitalWrite(DCmotorMiddleBI2, HIGH);
  digitalWrite(DCmotorBackBI1, LOW);        // Back Left Forward
  digitalWrite(DCmotorBackBI2, HIGH);
}

void DCLeft()
{
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
}

void DCRight()
{
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
}

void DCForward()
{
  Serial2.println("DC Moving forward");
  LightsOut(); //Turns all lights off

  // Set speed of all motors to 100%
  digitalWrite(DCmotorFrontPWMA, HIGH);     // Front Right Motor
  digitalWrite(DCmotorFrontPWMB, HIGH);     // Front Left Motor
  digitalWrite(DCmotorMiddlePWMA, HIGH);    // Middle Right Motor
  digitalWrite(DCmotorMiddlePWMB, HIGH);    // Middle Left Motor
  digitalWrite(DCmotorBackPWMA, HIGH);      // Back Right Motor
  digitalWrite(DCmotorBackPWMB, HIGH);      // Back Left Motor

  // All Right Motors move forward
  digitalWrite(DCmotorFrontAI1, LOW);       // Front Right Forward
  digitalWrite(DCmotorFrontAI2, HIGH);
  digitalWrite(DCmotorMiddleAI1, LOW);      // Middle Right Forward
  digitalWrite(DCmotorMiddleAI2, HIGH);
  digitalWrite(DCmotorBackAI1, LOW);        // Back Right Forward
  digitalWrite(DCmotorBackAI2, HIGH);

  // All Left Motors move forward
  digitalWrite(DCmotorFrontBI1, LOW);       // Front Left Forward
  digitalWrite(DCmotorFrontBI2, HIGH);
  digitalWrite(DCmotorMiddleBI1, LOW);      // Middle Left Forward
  digitalWrite(DCmotorMiddleBI2, HIGH);
  digitalWrite(DCmotorBackBI1, LOW);        // Back Left Forward
  digitalWrite(DCmotorBackBI2, HIGH);

}

void DCBack(int ReverseTime)
{
  Serial2.print("Reversing for: \t");
  Serial2.println(ReverseTime);

  int NumLed = 1; //Starts LED switch case ar 1
  unsigned long targetTime = millis();
  unsigned long currentTime = millis();
  unsigned long prevTime = millis();
  while (currentTime - targetTime < ReverseTime) { //Move backwards for the amount defined by ReverseTime
    currentTime = millis();

    // Reverse LED animation
    if (currentTime - prevTime > 200) {
      switch (NumLed) {
        case 0:
          for (byte pin = A5; pin >= A0; pin--) {
            digitalWrite(pin, HIGH);
          }
          NumLed = 1;
          break;

        case 1:
          for (byte pin = A5; pin >= A0; pin--) {
            digitalWrite(pin, LOW);
          }
          NumLed = 0;
          break;

      }
      prevTime = currentTime;
    }

    // Set speed of all motors to 100%
    digitalWrite(DCmotorFrontPWMA, HIGH);     // Front Right Motor
    digitalWrite(DCmotorFrontPWMB, HIGH);     // Front Left Motor
    digitalWrite(DCmotorMiddlePWMA, HIGH);    // Middle Right Motor
    digitalWrite(DCmotorMiddlePWMB, HIGH);    // Middle Left Motor
    digitalWrite(DCmotorBackPWMA, HIGH);      // Back Right Motor
    digitalWrite(DCmotorBackPWMB, HIGH);      // Back Left Motor

    // Move All Right Motors Backwards
    digitalWrite(DCmotorFrontAI1, HIGH);      // Front Right Backwards
    digitalWrite(DCmotorFrontAI2, LOW);
    digitalWrite(DCmotorMiddleAI1, HIGH);     // Middle Right Backwards
    digitalWrite(DCmotorMiddleAI2, LOW);
    digitalWrite(DCmotorBackAI1, HIGH);       // Back Right Backwards
    digitalWrite(DCmotorBackAI2, LOW);

    // Move All Left Motors Backwards
    digitalWrite(DCmotorFrontBI1, HIGH);      // Front Left Backwards
    digitalWrite(DCmotorFrontBI2, LOW);
    digitalWrite(DCmotorMiddleBI1, HIGH);     // Middle Left Backwards
    digitalWrite(DCmotorMiddleBI2, LOW);
    digitalWrite(DCmotorBackBI1, HIGH);       // Back Left Backwards
    digitalWrite(DCmotorBackBI2, LOW);
  }
  DCStop();
}

void DCStop()
{
  // Set speed of all motors to 0%
  digitalWrite(DCmotorFrontPWMA, LOW);
  digitalWrite(DCmotorFrontPWMB, LOW);
  digitalWrite(DCmotorMiddlePWMA, LOW);
  digitalWrite(DCmotorMiddlePWMB, LOW);
  digitalWrite(DCmotorBackPWMA, LOW);
  digitalWrite(DCmotorBackPWMB, LOW);

  // Stopping LED animation
  for (int j = 0; j <= 1; j++) {
    for (byte pin = A5; pin >= A0; pin--) {
      digitalWrite(pin, HIGH);
    }
    delay(100);
    for (byte pin = A5; pin >= A0; pin--) {
      digitalWrite(pin, LOW);
    }
    delay(100);
  }
  for (byte pin = A5; pin >= A0; pin--) {
    digitalWrite(pin, HIGH);
  }
}
