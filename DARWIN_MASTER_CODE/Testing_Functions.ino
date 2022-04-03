

void StepperOffset(){
  CenterStepper();
delay(5000);
StepperLeft(30);
delay(1000);
StepperRight(44);
delay(1000);
}

void FloorFinder() {


    LevelTOF(0);

    Serial2.print("\tdistance:\t");
    Serial2.println(GetDistance());
    delay(50);

}

void LevelTOFTest(){

  Serial2.print("\nIMUPitch:\t");
  Serial2.print(IMUPitch());
  Serial2.print("\tDegToServo:\t");
  Serial2.print(DegToServo(IMUPitch()));
  LevelTOF(0);
}

void ServoUpDownValues() { // Moves the servo up and down and outputs the degrees being sent and the values being written
    ServoPos(0);
  delay(3000);
  for (int i = 0; i < 60; i++) {
    Serial2.print("DegtoServo(");
    Serial2.print(i);
    Serial2.print("):\t");
    Serial2.println(DegToServo(i));
    ServoPos(i);
    delay(100);
  }
  delay(3000);
  for (int i = 0; i > -60; i--) {
    Serial2.print("DegtoServo(");
    Serial2.print(i);
    Serial2.print("):\t");
    Serial2.println(DegToServo(i));
    ServoPos(i);
    delay(100);
  }
  delay(3000);
}
