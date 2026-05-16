void calibrateMotorDrift() {
  //Serial.println("Starting drift calibration...");
  
  // Reset encoders
  leftCount = 0;
  rightCount = 0;

  // Run both motors at fixed PWM
  int testPWM = 120;
  setMotorL(testPWM);
  setMotorR(testPWM);

  unsigned long start = millis();
  unsigned long duration = 1000; // test for 1 second

  while (millis() - start < duration) {
    // just run motors
  }

  // Stop motors
  setMotorL(0);
  setMotorR(0);

  // Read how far each wheel turned
  long L = abs(leftCount);
  long R = abs(rightCount);

  if (R == 0) {
    return;
  }

  if (L == 0) {
    return;
  }

  // Trim calculation
  motorTrim = (float)R / (float)L;
}
