//Make sure to download the library Stepper incase you dont have it
//Pin layout: IN1 - 8, IN2 - 9, IN3 - 10, IN4 - 11

#include <Stepper.h>

#define steps 64
#define degree 5.625
int sped = 600; //fastest speed motor will allow
Stepper ralph(steps, 8, 10, 9, 11); //stepper pins


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //move 45 degrees forward
  ralph.setSpeed(sped); //10 rpm
  ralph.step(degToSteps(45)); // 1 rotation
  delay(1000);
  
  //move 45 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-45));
  delay(1000);

  //move 90 degrees forward
  ralph.setSpeed(sped); //10 rpm
  ralph.step(degToSteps(90)); // 1 rotation
  delay(1000);

  //move 90 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-90));
  delay(1000);

  //move 180 degrees forward
  ralph.setSpeed(sped); //10 rpm
  ralph.step(degToSteps(180)); // 1 rotation
  delay(1000);

  //move 180 degrees backward
  ralph.setSpeed(sped);
  ralph.step(degToSteps(-180));
  delay(1000);

  //move 360 degrees forward
  ralph.setSpeed(sped); //10 rpm
  ralph.step(degToSteps(360)); // 1 rotation
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
