void loop() {

 // Asking user to set a value to move the servo to
  Serial.println("Please enter a value to set the servo to: ");
  while (Serial.available() == 0){
  }
  int pos = Serial.parseInt();
  Serial.print("Setting servo to ");
  Serial.println(pos);
  TOFServo.write(pos);   // pos = user input here
  delay(1000);
Serial.println("\t distance : \t");
Serial.print(GetDistance());
}
