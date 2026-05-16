void line_following(){
  for (uint8_t i = 0; i < N; i++) {
    float v = readAnalogSample(analogPins[i]);
    norm[i] = normalizeValue(i, v);
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

void leftEncoderA() { if (digitalRead(encB_L)) leftCount--; else leftCount++; }
void leftEncoderB() { if (digitalRead(encA_L)) leftCount++; else leftCount--; }

void rightEncoderA() { if (digitalRead(encB_R)) rightCount++; else rightCount--; }
void rightEncoderB() { if (digitalRead(encA_R)) rightCount--; else rightCount++; }

float calculateLineError() {
  int weights[8] = {-3, -2, -1, 0, 0, 1, 2, 3};

  float sum = 0;
  float sumWeights = 0;

  for (int i = 0; i < 8; i++) {
    sum += (norm[i] * weights[i]);
    sumWeights += norm[i];
  }

  if (sumWeights == 0) return 0;  // no line found

  return sum / sumWeights;
}

float linePID(float error) {
  integral += error;
  float derivative = error - lastError;
  lastError = error;

  return Kp * error + Ki * integral + Kd * derivative;
}

int speedPID(int target, int measured, float Kp, float Ki, float Kd, int &prevErr, float &iTerm) {
  int err = target - measured;
  iTerm += err;
  int dTerm = err - prevErr;
  prevErr = err;

  return Kp*err + Ki*iTerm + Kd*dTerm;
}

void setMotorR(int pwm) {
  if (pwm > 0) {                // Forward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, pwm);

  } else if (pwm < 0) {         // Reverse
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, -pwm);

  } else {                      // Stop (coast)
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 0);
  }
}


void setMotorL(int pwm) {
  if (pwm > 0) {                // Forward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, pwm);

  } else if (pwm < 0) {         // Reverse
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, -pwm);

  } else {                      // Stop (coast)
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, 0);
  }
}





