// Input a value in degrees to set the servo to.
float ServoPos(int deg) {               // Send this function a value in degrees + for up, - for down
  TOFServo.write(DegToServo(deg));      // Pass that value to other functions to convert it and then write the value they return.
}



// Levels the TOF sensor based on the IMU's pitch reading
void LevelTOF() {
  int down = -64;                               // value to offset degrees by
  ServoPos(DegToServo(IMUPitch() + Offset));     // Same as float ServoPos() except this time we pass it the pitch value from the IMU instead of another degree.
}



// Convers a degree above ground level to a real value to set the servo to
float DegToServo(float deg) {         // Send this function the degrees above ground level you wish to set the servo to. - for down, + for up. 0 = straight in front
  int servoPos = 0;                   // Create an INT named servoPos to store our value of the servo in
  servoPos = (deg + 50.4) / 0.56;     // x = (y-b)/m math to calculate the value to send servo to achieve a speciffic degree
  if (servoPos < 0) {                 // filter results to stay within bounds of servo.
    servoPos = 0;                     // if servoPos is < 0, set it equal to 0
  }
  if (servoPos > 180) {               // if servoPos is > 180, set it equal to 180
    servoPos = 180;
  }
  return servoPos;                    // Return the value to set the servo to
}                                     // End of DegToServo()



// Convert servos position value in to degrees above ground level. 0 is straight ahead, - is down, + is up.
float ServoToDeg() {                  // Send this function the servos coded position
  float deg = 0;                      // Create a float named deg to store out value in degrees
  float servoPos = 0;                 // Create a FLOAT named servoPos to store out servos current position
  servoPos = TOFServo.read();         // read the servo and get the last position written
  deg = (0.56 * servoPos ) - 50.4;    // y = mx+b math to calculate the real value in degrees the servo is set to.
  return deg;                         // Return the value of degrees as a float. - for down, + for up. 0 for straight ahead
}                                     // End of ServoToDeg()
