/* Stepper Motor Driver Tutorial

*** Wiring Instructions ***
Arduino Pin 8 --- Driver 1N1
Arduino Pin 9 --- Driver 1N2
Arduino Pin 10 --- Driver 1N3
Arduino Pin 11 --- Driver 1N4

*/

#include <Stepper.h>    // Stepper motor library (Included with Arduino IDE)

#define steps 64                      // Setting the number of steps per rotation the motor takes. This is mechanically 
#define degree 5.625                  // Setting the number of degrees the motor rotates per step
int sped = 600;                       //fastest speed motor will allow
Stepper ralph(steps, 8, 10, 9, 11);   //stepper pins


void setup() {
}

void loop() {
  //move 45 degrees forward
  ralph.setSpeed(sped);         //10 rpm
  ralph.step(degToSteps(45));   // 1 rotation
  delay(1000);
  
  //move 45 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-45));
  delay(1000);

  //move 90 degrees forward
  ralph.setSpeed(sped);         //10 rpm
  ralph.step(degToSteps(90));   // 1 rotation
  delay(1000);

  //move 90 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-90));
  delay(1000);

  //move 180 degrees forward
  ralph.setSpeed(sped);         //10 rpm
  ralph.step(degToSteps(180));  // 1 rotation
  delay(1000);

  //move 180 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-180));
  delay(1000);

  //move 360 degrees forward
  ralph.setSpeed(sped);         //10 rpm
  ralph.step(degToSteps(360));  // 1 rotation
  delay(1000);

  //move 360 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-360));
  delay(1000);

}

//functon that takes in degrees requested and return steps needed
float degToSteps(float deg) { 
  return ((steps / degree) * deg) / 2;
}
