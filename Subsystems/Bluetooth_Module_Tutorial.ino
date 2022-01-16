//Make sure to download the SoftwareSerial library incase you don't have it
//Bluetooth module RX connected to arduino TX and bluetoothe TX connected to arduino RX
//Before uplaoding unplug RX pin, replug after uploaded succesfully
#include <SoftwareSerial.h>
SoftwareSerial EEBlue(0, 1); // RX | TX
void setup()
{
 
  Serial.begin(9600);
  EEBlue.begin(9600);  //Default Baud for comm, it may be different for your Module. 
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
