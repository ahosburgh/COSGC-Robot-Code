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
