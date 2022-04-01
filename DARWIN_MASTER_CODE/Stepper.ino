//functon that takes in degrees requested and return steps needed
void CenterStepper() {

  Serial2.println("\nCentering Stepper\n");
  ServoPos(90);
  delay(100);
  stepperAngle = 0;
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(20);
    delay(100);
  }
  StepperRight(30);
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(1);
    delay(100);
  }
  while (GetDistance() < 40) {
    StepperRight(1);
        delay(100);
  }


  StepperRight(155);                // Jump to right side and measure
  int stepperAngle1 = 155;

  while (GetDistance() > 40) {
    StepperRight(1);
    stepperAngle1 = stepperAngle1 + 1;
        delay(100);
  }
  while (GetDistance() < 40) {
    StepperLeft(1);
    stepperAngle1--;
        delay(100);
  }

  StepperLeft(155);                 // Jump back to left side
  int stepperAngle2 = 155;
  while (GetDistance() > 40) {
    StepperLeft(1);
    stepperAngle2 = stepperAngle2 + 1;
        delay(100);
  }
  while (GetDistance() < 40) {
    StepperRight(1);
    stepperAngle2--;
        delay(100);
  }
  stepperAngle = (stepperAngle1 + stepperAngle2) / 2;
  fastCenter = (stepperAngle / 2) + centerOffset;
  StepperRight((stepperAngle / 2) + centerOffset);
  stepperAngle = 0;
  LevelTOF();
  delay(100);
}

void FastCenter() {
  Serial2.println("\nFast Center\n");
  ServoPos(90);
  delay(100);
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(20);
  }
  StepperRight(30);
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(1);
    delay(100);
  }
  while (GetDistance() < 40) {
    StepperRight(1);
    delay(100);
  }
  StepperRight(fastCenter);
  LevelTOF();
  delay(100);
}




int degToSteps(int deg) {
  return ((steps / degree) * deg) / 2;
}

void StepperLeft(int deg){
  TOFStepper.step(degToSteps(-deg));
}

void StepperRight(int deg){
  TOFStepper.step(degToSteps(deg));
}
