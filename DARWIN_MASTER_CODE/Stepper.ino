//functon that takes in degrees requested and return steps needed
float degToSteps(float deg) {
  return ((steps / degree) * deg) / 2;
}

void StepperLeft(float deg){
  TOFStepper.step(degToSteps(-deg));
}

void StepperRight(float deg){
  TOFStepper.step(degToSteps(deg));
}
