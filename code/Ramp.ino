void ramp(){
  float dt = (millis() - lastTime_move_straight) / 1000.0;
  lastTime_move_straight = millis();

  pitch = read_angle();

// 2. Ramp Detection
  // -------------------------------
  bool climbing  = (pitch >  pitchThreshold_climb);
  bool descending = (pitch < -pitchThreshold_descend);


  // 3. Adjust Target Speed
  float desiredSpeed = targetSpeed;

  if (climbing) {
    // Boost speed for climbing
    desiredSpeed += rampUpBoost;
  }
  else if (descending) {
    // Reduce speed to avoid runaway acceleration
    desiredSpeed -= rampDownSlow;  

    if (desiredSpeed < minDownhillSpeed)
      desiredSpeed = minDownhillSpeed;
  }

  // 4. Acceleration Limiter
  float delta = desiredSpeed - currentSpeed;
  delta = constrain(delta, -accLimit, accLimit);
  currentSpeed += delta;


  // 5. Compute Encoder Velocity
  float measuredSpeed = getWheelSpeed(); 

  // 6. PID Speed Control
  float integral_ramp = 0;
  float lastError_ramp = 0;
  float speedError = currentSpeed - measuredSpeed;
  integral_ramp += speedError * dt;
  float derivative = (speedError - lastError) / dt;
  lastError_ramp = speedError;

  float pidOutput = Kp_speed * speedError + Ki_speed * integral + Kd_speed * derivative;

  if (climbing) {
    // Add torque compensation (forward)
    pidOutput += kRamp * sin(pitch * PI / 180.0);
  }

  if (descending) {
    // Apply braking force opposite direction
    pidOutput -= kDown * sin(abs(pitch) * PI / 180.0);
  }

  // 8. Constrain and Drive Motors
  pidOutput = constrain(pidOutput, 0, 255);
  setMotorL(pidOutput);
  setMotorR(pidOutput);
}

float getWheelSpeed() {

  static long lastLeft = 0;
  static long lastRight = 0;

  unsigned long now = millis();

  if (lastTime_speed == 0) {
    lastTime_speed = now;
    lastLeft = leftCount;
    lastRight = rightCount;
    return 0;  // first call, no speed yet
  }

  float dt = (now - lastTime_speed) / 1000.0;   // seconds
  if (dt <= 0) return 0;

  lastTime_speed = now;

  // Read encoder counts
  long l = leftCount;
  long r = rightCount;

  long dl = l - lastLeft;
  long dr = r - lastRight;

  lastLeft = l;
  lastRight = r;

  // Average ticks from both wheels
  float ticks = (abs(dl) + abs(dr)) * 0.5;

  // ---- Convert encoder ticks → linear speed ----
  float revs = ticks / CPR;                // wheel revolutions
  float distance = revs * wheelCirc;       // meters moved
  float speed = distance / dt;             // m/s

  return speed;   // ---- final speed in m/s ----
}
