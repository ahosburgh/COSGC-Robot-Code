void StartUpLights()
{
  for (byte pin = A0; pin < A0 + 6; pin++) {      // Setting all pin modes to output via for loop
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // Startup light show
  for (int j = 0; j <= 3; j++) {
    for (byte pin = A0; pin < A0 + 6; pin++) {
      digitalWrite(pin, LOW);
      delay(80);
      digitalWrite(pin, HIGH);
    }
  }
  delay(300);
  for (byte pin = A5; pin >= A0; pin--) {
    digitalWrite(pin, LOW);
    delay(100);
  }
}

//MOVE FORWARD ANIMATION (ALL LIGHTS OFF)
void LightsOut()
{
  for (byte pin = A5; pin >= A0; pin--) {
    digitalWrite(pin, LOW);
  }
}
