#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>

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
   2015/AUG/27  - Added calibration and system status helpers
   2015/NOV/13  - Added calibration save and restore
   */

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

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
    Serial2.println("------------------------------------");
    Serial2.print("Sensor:       "); Serial2.println(sensor.name);
    Serial2.print("Driver Ver:   "); Serial2.println(sensor.version);
    Serial2.print("Unique ID:    "); Serial2.println(sensor.sensor_id);
    Serial2.print("Max Value:    "); Serial2.print(sensor.max_value); Serial2.println(" xxx");
    Serial2.print("Min Value:    "); Serial2.print(sensor.min_value); Serial2.println(" xxx");
    Serial2.print("Resolution:   "); Serial2.print(sensor.resolution); Serial2.println(" xxx");
    Serial2.println("------------------------------------");
    Serial2.println("");
    delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
    */
/**************************************************************************/
void displaySensorStatus(void)
{
    /* Get the system status values (mostly for debugging purposes) */
    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    bno.getSystemStatus(&system_status, &self_test_results, &system_error);

    /* Display the results in the Serial Monitor */
    Serial2.println("");
    Serial2.print("System Status: 0x");
    Serial2.println(system_status, HEX);
    Serial2.print("Self Test:     0x");
    Serial2.println(self_test_results, HEX);
    Serial2.print("System Error:  0x");
    Serial2.println(system_error, HEX);
    Serial2.println("");
    delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
    */
/**************************************************************************/
void displayCalStatus(void)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    Serial2.print("\t");
    if (!system)
    {
        Serial2.print("! ");
    }

    /* Display the individual values */
    Serial2.print("Sys:");
    Serial2.print(system, DEC);
    Serial2.print(" G:");
    Serial2.print(gyro, DEC);
    Serial2.print(" A:");
    Serial2.print(accel, DEC);
    Serial2.print(" M:");
    Serial2.print(mag, DEC);
}

/**************************************************************************/
/*
    Display the raw calibration offset and radius data
    */
/**************************************************************************/
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial2.print("Accelerometer: ");
    Serial2.print(calibData.accel_offset_x); Serial2.print(" ");
    Serial2.print(calibData.accel_offset_y); Serial2.print(" ");
    Serial2.print(calibData.accel_offset_z); Serial2.print(" ");

    Serial2.print("\nGyro: ");
    Serial2.print(calibData.gyro_offset_x); Serial2.print(" ");
    Serial2.print(calibData.gyro_offset_y); Serial2.print(" ");
    Serial2.print(calibData.gyro_offset_z); Serial2.print(" ");

    Serial2.print("\nMag: ");
    Serial2.print(calibData.mag_offset_x); Serial2.print(" ");
    Serial2.print(calibData.mag_offset_y); Serial2.print(" ");
    Serial2.print(calibData.mag_offset_z); Serial2.print(" ");

    Serial2.print("\nAccel Radius: ");
    Serial2.print(calibData.accel_radius);

    Serial2.print("\nMag Radius: ");
    Serial2.print(calibData.mag_radius);
}


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
    */
/**************************************************************************/
void setup(void)
{
  //Bluetooth Serial2 Communication
  Serial2.begin(9600);                                              // Begin bluetooth communication at 9600
  Serial2.println("Bluetooth Serial2 Communication Established");    // Confirm bluetooth connection established
  Serial2.println(" ");
  // Pin TX2 and RX2 -- Yellow and Blue
    delay(1000);
    Serial2.println("Orientation Sensor Test"); Serial2.println("");

    /* Initialise the sensor */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial2.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }

    int eeAddress = 0;
    long bnoID;
    bool foundCalib = false;

    EEPROM.get(eeAddress, bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    /*
    *  Look for the sensor's unique ID at the beginning oF EEPROM.
    *  This isn't foolproof, but it's better than nothing.
    */
    bno.getSensor(&sensor);
    /*if (bnoID != sensor.sensor_id)
    {
        Serial2.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    }
    else
    {
        Serial2.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

        displaySensorOffsets(calibrationData);

        Serial2.println("\n\nRestoring Calibration data to the BNO055...");
        bno.setSensorOffsets(calibrationData);

        Serial2.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }*/

    delay(1000);

    /* Display some basic information on this sensor */
    displaySensorDetails();

    /* Optional: Display current status */
    displaySensorStatus();

   /* Crystal must be configured AFTER loading calibration data into BNO055. */
    bno.setExtCrystalUse(true);

    sensors_event_t event;
    bno.getEvent(&event);
    /* always recal the mag as It goes out of calibration very often */
    /*if (foundCalib){
        Serial2.println("Move sensor slightly to calibrate magnetometers");
        while (!bno.isFullyCalibrated())
        {
            bno.getEvent(&event);
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }
    }
    else
    {
    */
        Serial2.println("Please Calibrate Sensor: ");
        while (!bno.isFullyCalibrated())
        {
            bno.getEvent(&event);

            Serial2.print("X: ");
            Serial2.print(event.orientation.x, 4);
            Serial2.print("\tY: ");
            Serial2.print(event.orientation.y, 4);
            Serial2.print("\tZ: ");
            Serial2.print(event.orientation.z, 4);

            /* Optional: Display calibration status */
            displayCalStatus();

            /* New line for the next sample */
            Serial2.println("");

            /* Wait the specified delay before requesting new data */
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }
    //}

    Serial2.println("\nFully calibrated!");
    Serial2.println("--------------------------------");
    Serial2.println("Calibration Results: ");
    adafruit_bno055_offsets_t newCalib;
    bno.getSensorOffsets(newCalib);
    displaySensorOffsets(newCalib);

    Serial2.println("\n\nStoring calibration data to EEPROM...");

    eeAddress = 0;
    bno.getSensor(&sensor);
    bnoID = sensor.sensor_id;

    EEPROM.put(eeAddress, bnoID);

    eeAddress += sizeof(long);
    EEPROM.put(eeAddress, newCalib);
    Serial2.println("Data stored to EEPROM.");

    Serial2.println("\n--------------------------------\n");
    delay(500);
}

void loop() {
    /* Get a new sensor event */
    sensors_event_t event;
    bno.getEvent(&event);

    /* Display the floating point data */
    Serial2.print("X: ");
    Serial2.print(event.orientation.x, 4);
    Serial2.print("\tY: ");
    Serial2.print(event.orientation.y, 4);
    Serial2.print("\tZ: ");
    Serial2.print(event.orientation.z, 4);

    /* Optional: Display calibration status */
    displayCalStatus();

    /* Optional: Display sensor status (debug only) */
    //displaySensorStatus();

    /* New line for the next sample */
    Serial2.println("");

    /* Wait the specified delay before requesting new data */
    delay(BNO055_SAMPLERATE_DELAY_MS);
}
