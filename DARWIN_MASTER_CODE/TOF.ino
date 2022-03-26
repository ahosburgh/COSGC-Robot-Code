bool ObjectDetection(){

  int distance;
  int minDistance = 330;
  
  distance = GetDistance();
    if (distance > minDistance){
      Serial.print("Path Clear: \t"); 
      Serial.print(distance);
      Serial.println("\t");
      return false;
    }
    else
    {
      Serial.print("OBJECT DETECTED: \t");
      Serial.print(distance);
      Serial.println("\t");
      return true;
    }
}


// Uses the TOF sensor to measure a distance
int GetDistance() {
  
  int distance;                                                   // Creating and INT and naming it distance, to store the distance value recieved form the TOF sensor. 
  
  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  distance = measure.RangeMilliMeter;
//  if (measure.RangeMilliMeter < 8190)                             // IF the measurement is under the max value 
//  {                                                               //(8190 is the value returned when the TOF sensor measurements are out of range) 
//    distance = measure.RangeMilliMeter;                           // Set distance = the the measurement returned from the measurement function
//  }                                                               // End of IF 
//  else                                                            // ELSE
//  {
//    distance = measure.RangeMilliMeter;                           // Set distance = to the out of range value
//  }                                                               // End of ELSE 
  return distance;                                                // Return the value stored in distance
}                                                                 // End of GetDistance()


float ServoPos(int deg){
  TOFServo.write(DegToServo(deg));
}


// Convert servos position value to degrees above ground level 
float ServoToDeg(int servoPos){                                   // Send this function the servos coded position
  float deg;                                                      // Create a FLOAT named deg to store our value in degrees 
  deg = -0.4875 * servoPos + 133.875;                             // y = mx+b math to calculate the real value in degrees the servo is set to. 
  return deg;                                                     // Return the value of degrees as a float 
}                                                                 // End of ServoToDeg()



// Convers a degree above ground level to a real value to set the servo to 
float DegToServo(float deg){                                      // Send this function the degrees above ground level you wish to set the servo to
  int servoPos;                                                   // Create an INT named servoPos to store our value of the servo in
  servoPos = (deg - 133.875)/-0.4875;                             // x = (y-b)/m math to calculate the value to send servo to achieve a speciffic degree 
  return servoPos;                                                // Return the value to set the servo to 
}                                                                 // End of DegToServo()
