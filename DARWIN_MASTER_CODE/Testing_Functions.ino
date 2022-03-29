void FloorFinder() {

  for (int i = 90; i > 30; i++) {
    TOFServo.write(i);
//    Serial2.print(i);
//    Serial2.print("\tdistance:\t");
//    Serial2.println(GetDistance());
    delay(50);
  }
}

void LevelTOFTest(){
  TOFServo.write(45);
  Serial2.print("\nIMUPitch:\t");
  Serial2.println(IMUPitch());
//  Serial.print("\nDegToServo:\t");
  //Serial.print(DegToServo(IMUPitch()));
  //LevelTOF();
}
