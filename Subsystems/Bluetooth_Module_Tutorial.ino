/* HC-05 Bluetooth Module Tutorial

*** Wireing Instructions ***
Arduino Pin 5v --- HC-05 VCC
Arduino Pin GND --- HC-05 GND
Arduino Pin TX --- HC-05 RX
Arduino Pin RX --- HC-05 TX

*** Other Notes ***
Before uploading code, unplug the wire in the RX pin of the Arduino. Replug after uploading succesfully

*/

#include <SoftwareSerial.h>     
SoftwareSerial EEBlue(0, 1);    // RX | TX
void setup()
{
 
  Serial.begin(9600);
  EEBlue.begin(9600);           //Default Baud for comm, it may be different for your Module. 
  Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
 
}
 
void loop()
{
 
  // Feed any data from bluetooth to Terminal.
  if (EEBlue.available())
    Serial.write(EEBlue.read());
 
  // Feed all data from termial to bluetooth
  if (Serial.available())
    EEBlue.write(Serial.read());
}
