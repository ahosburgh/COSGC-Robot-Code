void setCal() {
  // DAVES MOD - Writes calibration data to sensor//
  byte calData;
  DarwinIMU.setMode( DarwinIMU.OPERATION_MODE_CONFIG );    // Put into CONFIG_Mode
  delay(25);

  calData = DarwinIMU.setCalvalARL(232);

  calData = DarwinIMU.setCalvalARM(3);

  calData = DarwinIMU.setCalvalMRL(79);

  calData = DarwinIMU.setCalvalMRM(3);

  calData = DarwinIMU.setCalvalAOXL(249);

  calData = DarwinIMU.setCalvalAOXM(255);

  calData = DarwinIMU.setCalvalAOYL(88);

  calData = DarwinIMU.setCalvalAOYM(0);

  calData = DarwinIMU.setCalvalAOZL(235);

  calData = DarwinIMU.setCalvalAOZM(255);

  calData = DarwinIMU.setCalvalMOXL(1);

  calData = DarwinIMU.setCalvalMOXM(2);

  calData = DarwinIMU.setCalvalMOYL(16);

  calData = DarwinIMU.setCalvalMOYM(3);

  calData = DarwinIMU.setCalvalMOZL(108);

  calData = DarwinIMU.setCalvalMOZM(7);

  calData = DarwinIMU.setCalvalGOXL(255);

  calData = DarwinIMU.setCalvalGOXM(255);

  calData = DarwinIMU.setCalvalGOYL(255);

  calData = DarwinIMU.setCalvalGOYM(255);

  calData = DarwinIMU.setCalvalGOZL(3);

  calData = DarwinIMU.setCalvalGOZM(0);

  DarwinIMU.setMode( DarwinIMU.OPERATION_MODE_NDOF );    // Put into NDOF Mode
  delay(25);
}



// IMU Roll function
// This function gets the current Roll from the IMU
float IMURoll()
{
  //NEW
  sensors_event_t event;
  DarwinIMU.getEvent(&event);
  return (event.orientation.z + 8);
}

// IMU Direction function
// This function gets the current direction from the IMU
float IMUDirection()
{
  //NEW
  sensors_event_t event;
  DarwinIMU.getEvent(&event);

  return (event.orientation.x);
}



// IMU Pitch function
// This function gets the current pitch angle from the IMU
float IMUPitch()
{
  //NEW
  sensors_event_t event;
  DarwinIMU.getEvent(&event);
  float pitch = (event.orientation.y);
  return pitch;
}




float GetGoldenDirection() {
  Serial2.println("\n==========Golden Direction==========\n");
  Serial2.println("Place Darwin on the ground and prepare for run");
  delay(3000);

  float sample1 = 0;
  float sample2 = 0;
  float sample3 = 0;
  int i = 9;
  unsigned long currentTime = millis();
  unsigned long oldTime = millis();

  Serial2.println("Taking Golden Direction Samples In: ");
  while (i > 0) {
    if (currentTime - oldTime > 1000)
    {
      Serial2.print("------------------------------\t");
      Serial2.println(i);
      i = i - 1;
      oldTime = currentTime;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();
  }

  sample1 = IMUDirection();
  Serial2.print("-------------------- Sample1: \t");
  Serial2.println(sample1);
  oldTime = currentTime;

  currentTime = millis();
  oldTime = millis();
  i = 1;

  while (i > 0) {
    if (currentTime - oldTime > 1000)
    {
      sample2 = IMUDirection();
      Serial2.print("-------------------- Sample2: \t");
      Serial2.println(sample2);
      oldTime = currentTime;
      i = 0;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();
  }

  currentTime = millis();
  oldTime = millis();
  i = 1;

  while (i > 0) {
    if (currentTime - oldTime > 1000)
    {
      sample3 = IMUDirection();
      Serial2.print("-------------------- Sample3: \t");
      Serial2.println(sample3);
      oldTime = currentTime;
      i = 0;
    }
    Serial2.print("Current Direction: \t");
    Serial2.println(IMUDirection());
    currentTime = millis();
  }

  GoldenDirection = (sample1 + sample2 + sample3) / 3;
  Serial2.print("Golden Direction set to: ");
  Serial2.println(GoldenDirection);
}
