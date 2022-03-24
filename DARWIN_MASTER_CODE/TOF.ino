float GetDistance(){
  float OldDistance = distance;
  VL53L0X_RangingMeasurementData_t measure;                                         // create TOF variable named measure
    
  Serial.println("GetDistance Function Successfully Called");    
  DarwinTOF.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeMilliMeter < 8190)   // 8190 is the value returned when the TOF sensor measurements are out of range
  {
    distance = measure.RangeMilliMeter;
    Serial.print("Distance (mm): "); Serial.println(distance);       // measure.RangeMilliMeter is the actual variable we will need
    
  } 
  else 
  {
    distance = OldDistance;
    Serial.print(" Out of Range: using OldDistance value : ");
    Serial.println(distance);
    
  } 
  
  return distance;
}


//Convert servos position value to degrees above ground level 
int ServoToDeg(int servoPos){
  float deg;
  deg = -0.4875 * servoPos + 133.875;

  return deg
}
