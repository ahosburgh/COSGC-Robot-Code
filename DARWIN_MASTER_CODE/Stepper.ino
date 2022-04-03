//functon that takes in degrees requested and return steps needed
void CenterStepper() {
  TOFStepper.setSpeed(600);
  Serial2.println("\nCentering Stepper\n");
  ServoPos(90);
  delay(50);

  stepperAngle = 0;
  while (GetDistance() > 50) {      // Find first left side
    StepperLeft(20);
    delay(50);
  }

  StepperRight(30);
  TOFStepper.setSpeed(100);
  while (GetDistance() > 50) {      // Find first left side
    StepperLeft(1);
    delay(50);
  }

  while (GetDistance() < 50) {
    StepperRight(1);
    delay(50);
  }

  TOFStepper.setSpeed(600);
  StepperRight(140);                // Jump to right side and measure
  int stepperAngle1 = 140;
  TOFStepper.setSpeed(100);
  while (GetDistance() > 50) {
    StepperRight(1);
    stepperAngle1 = stepperAngle1 + 1;
    delay(50);
  }
  while (GetDistance() < 50) {
    StepperLeft(1);
    stepperAngle1--;
    delay(50);
  }
  TOFStepper.setSpeed(600);
  StepperLeft(140);                 // Jump back to left side
  int stepperAngle2 = 140;
  TOFStepper.setSpeed(100);
  while (GetDistance() > 50) {
    StepperLeft(1);
    stepperAngle2 = stepperAngle2 + 1;
    delay(50);
  }
  while (GetDistance() < 50) {
    StepperRight(1);
    stepperAngle2--;
    delay(50);
  }
  stepperAngle = (stepperAngle1 + stepperAngle2) / 2;
  fastCenter = (stepperAngle / 2) + centerOffset;
  TOFStepper.setSpeed(600);
  StepperRight((stepperAngle / 2) + centerOffset);
  stepperAngle = 0;
  LevelTOF(0);
  delay(50);
}

void FastCenter() {
  Serial2.println("\nFast Center\n");
  ServoPos(90);
  delay(50);
  TOFStepper.setSpeed(600);

  while (GetDistance() > 50) {      // Find first left side
    StepperLeft(20);
    delay(50);
  }
  StepperRight(30);
  delay(50);
  TOFStepper.setSpeed(100);
  while (GetDistance() > 50) {      // Find first left side
    StepperLeft(1);
    delay(50);
  }
  while (GetDistance() < 50) {
    StepperRight(1);
    delay(50);
  }
  TOFStepper.setSpeed(600);
  StepperRight(fastCenter);
  LevelTOF(0);
  delay(50);
}



int degToSteps(int deg) {
  return ((steps / degree) * deg) / 2;
}

void StepperLeft(int deg) {
  TOFStepper.step(degToSteps(-deg));
}

void StepperRight(int deg) {
  TOFStepper.step(degToSteps(deg));
}
