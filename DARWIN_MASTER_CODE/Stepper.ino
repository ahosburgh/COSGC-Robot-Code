//functon that takes in degrees requested and return steps needed
void CenterStepper() {

  Serial2.println("\nCentering Stepper\n");
  ServoPos(0);
  delay(100);
  stepperAngle = 0;
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(20);
  }
  StepperRight(30);
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(1);
  }
  while (GetDistance() < 40) {
    StepperRight(1);
  }


  StepperRight(135);                // Jump to right side and measure
  int stepperAngle1 = 135;

  while (GetDistance() > 40) {
    StepperRight(1);
    stepperAngle1 = stepperAngle1 + 1;
  }
  while (GetDistance() < 40) {
    StepperLeft(1);
    stepperAngle1--;
  }

  StepperLeft(135);                 // Jump back to left side
  int stepperAngle2 = 135;
  while (GetDistance() > 40) {
    StepperLeft(1);
    stepperAngle2 = stepperAngle2 + 1;
  }
  while (GetDistance() < 40) {
    StepperRight(1);
    stepperAngle2--;
  }
  stepperAngle = (stepperAngle1 + stepperAngle2) / 2;
  fastCenter = (stepperAngle / 2) + 18;
  StepperRight((stepperAngle / 2) + 18);
  stepperAngle = 0;
  LevelTOF();
  delay(100);
}

void FastCenter() {
  Serial2.println("\nFast Center\n");
  ServoPos(0);
  delay(100);
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(20);
  }
  StepperRight(30);
  while (GetDistance() > 40) {      // Find first left side
    StepperLeft(1);
  }
  while (GetDistance() < 40) {
    StepperRight(1);
  }
  StepperRight(fastCenter);
  LevelTOF();
  delay(100);
}




float degToSteps(float deg) {
  return ((steps / degree) * deg) / 2;
}

void StepperLeft(float deg){
  TOFStepper.step(degToSteps(-deg));
}

void StepperRight(float deg){
  TOFStepper.step(degToSteps(deg));
}
