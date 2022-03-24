void MoveForward()
{
  Serial.println("Moving forward");
  LightsOut();

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

void MoveBack()
{
  int NumLed = 1;
  unsigned long targetTime = millis();
  unsigned long currentTime = millis();
  unsigned long prevTime = millis();
  while (currentTime - targetTime < 3000) {
    currentTime = millis();
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
