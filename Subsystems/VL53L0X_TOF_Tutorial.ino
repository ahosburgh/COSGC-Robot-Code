/* Time of Flight Sensor Tuitorial
 
 *** Required Libraries to install in the Arduino IDE ***
 Adafruit_VL53L0X-master.zip
 
 *** Wiring Instructions *** 
Arduino 5v --- TOF VIN
Arduino GND --- TOF GND
Arduino A4 --- TOF SDA
Arduino A5 --- TOF SCL

***Other Notes ***
Set Serial Monitor baud rate to 115200 
Remove film protecting laser before using. 
*/

#include <Adafruit_VL53L0X.h>                 // VL53L0X TOF Sensor Library

Adafruit_VL53L0X TOF = Adafruit_VL53L0X();    // Creating a new Adafruit_VL53L0X object named "TOF" (this is the sensors name)

void setup() 
{
  Serial.begin(115200);                       // Establish Serial communication at a baud rate of 115200 
  TOF.begin();                                // Establish connection with TOF sensor
}

void loop() 
{
  //Some of this stuff I can't find exact documentation for, but the sensor doesn't work right without it, so... we leave it in
  
  VL53L0X_RangingMeasurementData_t measure;                                         // idk. I assume "begin measurement function" 
    
  Serial.print("Reading a measurement... ");    
  TOF.rangingTest(&measure, false); // pass in 'true' to get debug data printout!   // idk. I assume "take measurement" 

  if (measure.RangeStatus != 4)   // phase failures have incorrect data             // idk. Also required for accurate measurements though
  {
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);       // measure.RangeMilliMeter is the actual variable we will need
  } 
  else 
  {
    Serial.println(" out of range ");
  }
    
  delay(100);
}
