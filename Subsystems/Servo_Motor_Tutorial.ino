/* Basic Servo Motor Tuitorial 
  
*** Required Libraries to install in Arduino IDE **
Servo.h (Included in Arduino IDE)

*** Wiring Instructions ***
Arduino Pin 5v --- Servo Wire Red
Arduino Pin GND --- Servo Wire Black
Arduino Pin (Any Digital Pin) --- Servo Wire Yellow

*** Other Notes ****
These types of servos do NOT move fully from 0 to 180 degrees. The limits of its range of motion are as follows
 
*/

#include <Servo.h>    // Including the servo library 
Servo myServo;        // Creating a new servo object named myServo
int pos = 90;       // Creating int named pos and setting it to 90. This will be the degrees our servo turns.
int signalPin = 8;    // This is the pin that the signal wire is connect to the arduino through. Can be any digital out pin.

void setup() {
  Serial.begin(9600);
  myServo.attach(signalPin);    // Attaches the servo to the signalPin of the Arduino
  // Set servo to the "middle" of its range of motion
  //myServo.write(pos);  // pos = 45 here
  //delay(1000);         // wait for the servo to reach its destination
}
void loop() {
  // Asking user to set a value to move the servo to
  Serial.println("Please enter a value to set the servo to: ");
  while (Serial.available() == 0){
  }
  pos = Serial.parseInt();
  Serial.print("Setting servo to ");
  Serial.println(pos);
  myServo.write(pos);   // pos = user input here
  delay(1000);
}
