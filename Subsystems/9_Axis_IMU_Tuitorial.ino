/* 9 Axis IMU Tuitorial

*** Required Libraries to install in the Arduino IDE ***
Adafruit_BNO055-master.zip
Adafruit_Sensor-master.zip (also called AdaFruit_Unified_Sensor Library)
Adafruit_BusIO-master.zip (but not actually called in the sketch) 

*** Wiring instructions ***
Arduino Pin A4 --- IMY Pin SDA
Arduino Pin A5 --- IMU Pin SCL
Arduino Pin 5v --- IMU Pin VIN
Arudino Pin GND --- IMU Pin GND

*** Other Notes *** 
Set Serial Monitor baud rate to 115200

*/

#include <Wire.h>               // I2C comunication library (built into Arduino)
#include <Adafruit_Sensor.h>    // Adafruit Unified Sensor Library
#include <Adafruit_BNO055.h>    // Adafruit BNO055 Library 
#include <utility/imumaths.h>   // Utility included in one of the 2 libraries above, but needs to be called specifically

#define BN0055_SAMPLERATE_DELAY_MS (100)    // We want the sensor to sample every 100 ms

Adafruit_BNO055 myIMU = Adafruit_BNO055();  // Using adafruit library to create IMU object named myIMU

void setup() {

Serial.begin (115200);          // Connecting to serial monitor at a fast rate because we got a lot of data to push through
myIMU.begin();                  // Starting the IMU waiting 1 second to give it time to power on and make its connection before sending another command
delay(1000);                    // waiting 1 second to give it time to power on and make its connection before sending another command
/* int8_t is a special type of int variable type that stores values from -120 to 120. super compact. Some of the measurements from the IMU
 * are dependant on tempature. So we need to measure the tempature of the IMU first.
 * Se we will create a new variable named temp of type int8_t and set it = to a function return of the imu library that does exactly that.
*/
int8_t temp = myIMU.getTemp();
Serial.println(temp);           // Printing the measured tempature to the screen for sanity check. 
myIMU.setExtCrystalUse(true);   // Dont use the crystal on the chip itself, use crystal on the board (for time keeping)
}

void loop() {
// This is a line of code that uses the BNO055 library. 
// "imu:: calls out to the imu section of the library and Vector<3> is the number of vectors we are looking for. 
// acc is a new variable created by the library at this time and we are = it to our actual imu's vector readings using 
// another library command myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER)
imu::Vector<3> acc = myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
// ditto but gyro data
imu::Vector<3> gyro = myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
// ditto but magnet
imu::Vector<3> mag = myIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

// Printing data

Serial.print("Accelerometer: ");
Serial.print(acc.x());
Serial.print(", ");
Serial.print(acc.y());
Serial.print(", ");
Serial.print(acc.z());

Serial.print("   Gyro: ");
Serial.print(gyro.x());
Serial.print(", ");
Serial.print(gyro.y());
Serial.print(", ");
Serial.print(gyro.z());

Serial.print("   Magnetometer: ");
Serial.print(mag.x());
Serial.print(", ");
Serial.print(mag.y());
Serial.print(", ");
Serial.println(mag.z());

delay(BN0055_SAMPLERATE_DELAY_MS);
}
