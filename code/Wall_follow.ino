// --- PID constants for wall following ---
float Kp_wall = 0.5;
float Ki_wall = 0.0;
float Kd_wall = 0.5;

// --- PID constants for speed control ---
float Kp_speed_wall = 0.5;  // reduced from 0.5 to prevent overshoot
float Ki_speed_wall = 0.0;
float Kd_speed_wall = 0.0;

// --- Wall-following variables ---
int error_wall = 0;
int lastError_wall = 0;
long integral_wall = 0;

// --- Target distance and speed ---
int targetDistance_wall = 50; // mm

// --- Encoder tracking ---
volatile long prevLeft = 0;
volatile long prevRight = 0;
unsigned long lastTime_wall = 0;

// --- Speed PID variables ---
float integral_speed = 0;
float prevError_speed = 0;

void wall_following() {
    unsigned long now = millis();
    float dt = (now - lastTime_wall) / 1000.0;  // convert ms to seconds
    if (dt <= 0) return;  // avoid division by zero
    lastTime_wall = now;

    // --- Read ultrasonic distance ---
    long distance = readUltrasonicDistance2(); // returns mm

    // --- Wall-following PID ---
    error_wall = targetDistance_wall - distance;
    integral_wall += error_wall * dt;
    int derivative_wall = (error_wall - lastError_wall) / dt;
    lastError_wall = error_wall;

    int correction = Kp_wall * error_wall + Ki_wall * integral_wall + Kd_wall * derivative_wall;

    // --- Compute wheel speeds from encoders ---
    long dL = leftCount - prevLeft;
    long dR = rightCount - prevRight;
    prevLeft = leftCount;
    prevRight = rightCount;

    float vLeft = (dL / CPR) * wheelCirc / dt;
    float vRight = (dR / CPR) * wheelCirc / dt;
    float vAvg = (vLeft + vRight) / 2.0;

    // --- Speed PID ---
    float error_speed = targetSpeed - vAvg;
    integral_speed += error_speed * dt;
    // Anti-windup
    integral_speed = constrain(integral_speed, -100, 100);
    float derivative_speed = (error_speed - prevError_speed) / dt;
    prevError_speed = error_speed;

    float pwmSpeed = baseSpeed + Kp_speed_wall * error_speed + Ki_speed_wall * integral_speed + Kd_speed_wall * derivative_speed;

    // --- Combine speed and wall correction ---
    int leftPWM = constrain(pwmSpeed + correction, 0, maxPWM);
    int rightPWM = constrain(pwmSpeed - correction, 0, maxPWM);

    // --- Drive motors ---
    setMotorL(leftPWM);
    setMotorR(rightPWM);

    delay(100);
}
