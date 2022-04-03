// Uses the TOF sensor to measure a distance
int GetDistance() {
  int distance = 0;                                               // Creating and INT and naming it distance, to store the distance value recieved form the TOF sensor.

  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  distance = measure.RangeMilliMeter;

  return distance;                                                // Return the value stored in distance
}                                                                 // End of GetDistance()


int GetAvgDistance() {

  int avg = 0;
  int avgSize = 50;
  int avgArray[avgSize];
  int distance = 0;
  unsigned long currentTime = millis();        // Assigning variable to keep track of the time passing in milliseconds
  unsigned long prevTime = millis();      // Assigning variable to keep track of the time passing in milliseconds

  VL53L0X_RangingMeasurementData_t measure;                       // Create TOF variable named measure
  DarwinTOF.rangingTest(&measure, false);                         // Running the built in ranging function to get a measurement pass in 'true' to get debug data printout!

  for (int i = 0; i < avgSize; i++) {
    distance = measure.RangeMilliMeter;
    if (distance > 900) {
      while (distance > 900 && currentTime - prevTime < 5000) {   // run this loop to correct for misfirings in the tof, but only for 5 seconds max.
        distance = measure.RangeMilliMeter;                       // because if we are "holding" an infinity reading, then we're probably looking at infintiy.
        Serial2.print("GetAveDistance error! Reading:\t");
        Serial2.print(distance);
        currentTime = millis();
      }
      if (distance > 900) {
        return distance;
      }
    }
    avgArray[i] = distance;
  }

  for (int i = 0; i < avgSize; i++) {
    avg = avg + avgArray[i];
  }
  avg = avg / avgSize;

  return avg;                                                // Return the value stored in distance
}
