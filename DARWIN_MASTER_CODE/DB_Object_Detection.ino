// Uses the TOF sensor to measure a distance
int GetDistance() {
  int distance = 0;                                                   // Creating and INT and naming it distance, to store the distance value recieved form the TOF sensor.

  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  distance = measure.RangeMilliMeter;

  return distance;                                                // Return the value stored in distance
}                                                                 // End of GetDistance()


// Uses the TOF Sensor to determine if an object is found 
bool ObjectDetection() {

  int distance = 0;
  int minDistance = 600;

  distance = GetDistance();
  if (distance > minDistance) {
    Serial2.print("ObjectDetection() = FALSE \t Distance = \t");
    Serial2.print(distance);
    Serial2.print("\t");
    return false;
  }
  else
  {
    Serial2.print("ObjectDetection() = TRUE \t Distance = \t");
    Serial2.print(distance);
    Serial2.print("\t");
    return true;
  }
}
