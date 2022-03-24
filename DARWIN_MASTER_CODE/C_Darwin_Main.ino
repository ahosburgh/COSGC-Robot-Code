void loop() {

  DCForward();
  if (detect obstacle) {
    TurnLeft(45);
    if (detect obstacle) {
      TurnRight(90);
    }
  }
  DCForward();
  DCDrift();
}
