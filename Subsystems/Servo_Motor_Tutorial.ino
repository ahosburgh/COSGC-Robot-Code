/* Basic Servo Motor Tuitorial 

*** Wiring Instructions ***
Arduino Pin 5v --- Servo Wire Red
Arduino Pin GND --- Servo Wire Black
Arduino Pin (Any Digital Pin) --- Servo Wire Yellow
 
*/

#include <Servo.h>    // Including the servo library 
Servo myServo;        // Creating a new servo object named myServo
int pos = 90;         // Creating int named pos and setting it to 90. This will be the degrees our servo turns.
int signalPin = 8;    // This is the pin that the signal wire is connect to the arduino through. Can be any digital out pin.

void setup() 
{
  Serial.begin(9600);
  myServo.attach(signalPin);    // Attaches the servo to the signalPin of the Arduino
}

void loop() 
{
  Serial.println("Please enter a value to set the servo to: ");   // Asking user to set a value to move the servo to
  while (Serial.available() == 0)
  {
  }
  pos = Serial.parseInt();
  Serial.print("Setting servo to ");
  Serial.println(pos);
  myServo.write(pos);   // This is basically the only line of code in void loop() we will need. This is how you write a position to the servo. pos needs to = 0-180
  delay(1000);
}
