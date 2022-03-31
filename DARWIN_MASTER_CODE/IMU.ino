void setCal() {
  // DAVES MOD - Writes calibration data to sensor//
  byte calData;
  bno.setMode( bno.OPERATION_MODE_CONFIG );    // Put into CONFIG_Mode
  delay(25);

  calData = bno.setCalvalARL(232);

  calData = bno.setCalvalARM(3);

  calData = bno.setCalvalMRL(79);

  calData = bno.setCalvalMRM(3);

  calData = bno.setCalvalAOXL(249);

  calData = bno.setCalvalAOXM(255);

  calData = bno.setCalvalAOYL(88);

  calData = bno.setCalvalAOYM(0);

  calData = bno.setCalvalAOZL(235);

  calData = bno.setCalvalAOZM(255);

  calData = bno.setCalvalMOXL(1);

  calData = bno.setCalvalMOXM(2);

  calData = bno.setCalvalMOYL(16);

  calData = bno.setCalvalMOYM(3);

  calData = bno.setCalvalMOZL(108);

  calData = bno.setCalvalMOZM(7);

  calData = bno.setCalvalGOXL(255);

  calData = bno.setCalvalGOXM(255);

  calData = bno.setCalvalGOYL(255);

  calData = bno.setCalvalGOYM(255);

  calData = bno.setCalvalGOZL(3);

  calData = bno.setCalvalGOZM(0);

  bno.setMode( bno.OPERATION_MODE_NDOF );    // Put into NDOF Mode
  delay(25);
}



// IMU Roll function
// This function gets the current Roll from the IMU
float IMURoll()
{
  //NEW
  sensors_event_t event;
  bno.getEvent(&event);
  setCal();
  float roll = (event.orientation.z, 4);
  roll  = roll + 8;
  return (event.orientation.z, 4);
}

// IMU Direction function
// This function gets the current direction from the IMU
float IMUDirection()
{
  //NEW
  sensors_event_t event;
  bno.getEvent(&event);
  setCal();
  return (event.orientation.x, 4);
}



// IMU Pitch function
// This function gets the current pitch angle from the IMU
float IMUPitch()
{
  //NEW
  sensors_event_t event;
  bno.getEvent(&event);
  setCal();
  return (event.orientation.y, 4);
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
