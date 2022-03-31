#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>
/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3-5V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
   2015/Mar/12  - Dave's mod - calibration
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
float thetaM = 0;           // Pitch from Accelerometer
float phiM = 0;             // Roll from Accellerometer
float thetaFold = 0;    // Overall System Pitch Filtered (Old)
float thetaFnew = 0;        // Overall System Pitch Filtered (New)
float phiFold = 0;      // Overall System Roll Filtered  (Old)
float phiFnew = 0;          // Overall System Roll Filtered (New)
float theta = 0;            // Overall System Pitch
float phi = 0;              // Overall System Roll
float thetaRad = 0;         // Pitch in radians
float phiRad = 0;           // Roll in radians
float Xm = 0;               // Value coming off the x magnitometer
float Ym = 0;               // Value coming off the Y Magnitometer
float psi = 0;              // Heading angle
float dt = 0;               // Delta time
unsigned long millisOld = 0;// Old value of time in milliseconds
float GoldenDirection = 0;  // The direction if the final goal
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  setCal();       // Set Calibration Values - Comment out to read calibration values
  delay(1000);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  bno.setExtCrystalUse(true);

}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  //getCalStat();                  // Uncomment to get calibration values
//
//
//setCal();
    /* Display the floating point data */
    Serial.print("X: ");
    float x = (event.orientation.x);
    Serial.print(x);
    Serial.print("\tY: ");
    float y = (event.orientation.y);
    Serial.print(y);
    Serial.print("\tZ: ");
    float z = (event.orientation.z);
    Serial.print(z);
    Serial.println("");
  //Serial.println(IMUDirection());
  delay(BNO055_SAMPLERATE_DELAY_MS);

}
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

void getCal() {
  // Dave's Mod - Reads Calibration Data when sensors are calibrated
  byte calData;
  bno.setMode( bno.OPERATION_MODE_CONFIG );    // Put into CONFIG_Mode

  calData = bno.getCalvalARL();
  Serial.println(calData);

  calData = bno.getCalvalARM();
  Serial.println(calData);

  calData = bno.getCalvalMRL();
  Serial.println(calData);

  calData = bno.getCalvalMRM();
  Serial.println(calData);

  calData = bno.getCalvalAOXL();
  Serial.println(calData);

  calData = bno.getCalvalAOXM();
  Serial.println(calData);

  calData = bno.getCalvalAOYL();
  Serial.println(calData);

  calData = bno.getCalvalAOYM();
  Serial.println(calData);

  calData = bno.getCalvalAOZL();
  Serial.println(calData);

  calData = bno.getCalvalAOZM();
  Serial.println(calData);

  calData = bno.getCalvalMOXL();
  Serial.println(calData);

  calData = bno.getCalvalMOXM();
  Serial.println(calData);

  calData = bno.getCalvalMOYL();
  Serial.println(calData);

  calData = bno.getCalvalMOYM();
  Serial.println(calData);

  calData = bno.getCalvalMOZL();
  Serial.println(calData);

  calData = bno.getCalvalMOZM();
  Serial.println(calData);

  calData = bno.getCalvalGOXL();
  Serial.println(calData);

  calData = bno.getCalvalGOXM();
  Serial.println(calData);

  calData = bno.getCalvalGOYL();
  Serial.println(calData);

  calData = bno.getCalvalGOYM();
  Serial.println(calData);

  calData = bno.getCalvalGOZL();
  Serial.println(calData);

  calData = bno.getCalvalGOZM();
  Serial.println(calData);

  while (1) {                            // Stop
    delay(1000);
  }


}
void getCalStat() {
  // Dave's Mod - Move sensor to calibrate, when status shows calibration, read values
  byte cal = bno.getCalib();
  byte calSys = (0xC0 & cal) >> 6;
  byte calGyro = (0x30 & cal) >> 4;
  byte calAccel = (0x0C & cal) >> 2;
  byte calMag = (0x03 & cal) >> 0;

  Serial.println(cal);
  Serial.print("System calibration status "); Serial.println(calSys);
  Serial.print("Gyro   calibration status "); Serial.println(calGyro);
  Serial.print("Accel  calibration status "); Serial.println(calAccel);
  Serial.print("Mag    calibration status "); Serial.println(calMag);

  delay(1000);
  if (cal == 255) {
    getCal();
  }
}

float IMUDirection()
{
  setCal();
  for (int i = 0; i < 20; i++) {                                         // Looping 10 times just to get a good average value
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    thetaM = -atan2(accel.x() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360;  // Math to get the pitch from the accelerometer
    phiM = -atan2(accel.y() / 9.8, accel.z() / 9.8) / 2 / 3.141592654 * 360;    // Math to get the roll from the accelerometer

    phiFnew = .95 * phiFold + .05 * phiM;                             // Filter for accelerometer roll data
    thetaFnew = .95 * thetaFold + .05 * thetaM;                       // Filter for accelerometer pitch data

    dt = (millis() - millisOld) / 1000.;                              // Measuring the change in time since last millis measurement
    millisOld = millis();                                             // Setting millisOld to millis to update it for next calculation
    theta = (theta + gyro.y() * dt) * .95 + thetaM * .05;             // Calculating Absolute Roll using fancy math (and running a filter on it)
    phi = (phi - gyro.x() * dt) * .95 + phiM * .05;                   // Calculating Absolute Pitch using fancy math  (and running a filter on it)

    phiRad = phi / 360 * (2 * 3.14);                                  // Calculating
    thetaRad = theta / 360 * (2 * 3.14);

    Xm = mag.x() * cos(thetaRad) - mag.y() * sin(phiRad) * sin(thetaRad) + mag.z() * cos(phiRad) * sin(thetaRad); // Absolute X Direction of magnitometer after fancy math
    Ym = mag.y() * cos(phiRad) + mag.z() * sin(phiRad);                                                 // Absolute Y Direction of magnitometer after fancy math

    psi = atan2(Ym, Xm) / (2 * 3.14) * 360;                           // Absolute heading of the robot

    phiFold = phiFnew;
    thetaFold = thetaFnew;
  }                                 // End of for loop
  return psi;     // Return the absolute heading
}

void AdamDir(){
      imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    
}
