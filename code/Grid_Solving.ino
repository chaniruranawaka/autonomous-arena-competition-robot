void grid(){
  Serial.println(" Inside grid");
  int cross_count = 0;
  while (cross_count < 7) {
    for (uint8_t i = 0; i < N; i++) {
    float v = readAnalogSample(analogPins[i]);
    norm[i] = normalizeValue(i, v);
    }

    // grid cross counting
    bool cross = true;
    for (uint8_t i = 0; i < N; i++) {
      if (norm[i] == 1) {
        cross = false;
        break;
      }
    }  

    // -------- Cross detection --------
    if(cross && !crossDetected &&
       (millis() - lastCrossTime > crossCooldown)) {

        cross_count++;
        crossDetected = true;
        lastCrossTime = millis();
    }

    if(!cross) {
        crossDetected = false;
    }

    // 1. Get line error
    error = calculateLineError();

    // 2. PID for steering
    float correction = linePID(error);

    // 3. Read encoder counts
    long leftSpeed = leftCount;
    long rightSpeed = rightCount;
    leftCount = rightCount = 0;  // reset after reading

    // 4. PID for wheel speed
    static int prevErrL = 0, prevErrR = 0;
    static float iTermL = 0, iTermR = 0;

    // 5. Final motor PWM
    int leftPWM  = baseSpeed - correction;
    int rightPWM = baseSpeed + correction;

    // 6. Limit PWM
    leftPWM  = constrain(leftPWM,  0, maxPWM);
    rightPWM = constrain(rightPWM, 0, maxPWM);

    // 7. Drive motors
    setMotorL(leftPWM);
    setMotorR(rightPWM);

    delay(20);
  }

  setMotorL(-1);
  setMotorR(-1);

  return;

}