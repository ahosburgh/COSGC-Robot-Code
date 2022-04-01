void FloorFinder() {


    LevelTOF();

    Serial2.print("\tdistance:\t");
    Serial2.println(GetDistance());
    delay(50);

}

void LevelTOFTest(){
  TOFServo.write(45);
  Serial2.print("\nIMUPitch:\t");
  Serial2.println(IMUPitch());
//  Serial.print("\nDegToServo:\t");
  //Serial.print(DegToServo(IMUPitch()));
  //LevelTOF();
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
