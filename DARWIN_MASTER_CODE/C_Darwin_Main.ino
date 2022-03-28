void loop() {
  ServoPos(0);

  while (GetDistance() > 50) {
    Serial2.println(GetDistance());
    StepperLeft(1);
  }
    Serial2.println(GetDistance());
    delay(10000);
}

// go towards the golden direction

// if obstacle is detected, measure it, and then decide to turn left or right

// turn left or right

// look around the side of the obstacle to see if we can see it anymore

// if we look clear then straighten out towards the golden direction

// check left and right to ensure we are clear of obstacles

// go towards the golden direction
