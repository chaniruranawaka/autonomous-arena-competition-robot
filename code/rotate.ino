void rotate_angle(float angleDeg, float targetSpeed) {

    // --- Reset encoders ---
    leftCount = 0;
    rightCount = 0;

    unsigned long lastTime = millis();

    // Direction from sign
    int dir = (angleDeg >= 0) ? 1 : -1;  // +1 = CCW, -1 = CW
    float absAngle = abs(angleDeg);

    // Compute wheel arc distance
    float arcLength = (PI * wheelBase * absAngle) / 360.0;
    long targetCounts = (arcLength / wheelCirc) * CPR;

    // --- PID variables ---
    float Kp_rotate = 10.0;
    float Ki_rotate = 0.2;
    float Kd_rotate = 7.0;

    float integral = 0;
    float prevError = 0;


    while (abs(leftCount) < targetCounts && abs(rightCount) < targetCounts) {


        unsigned long now = millis();
        float dt = (now - lastTime
        if (dt <= 0) continue;
        lastTime = now;
        

        // --- Compute wheel speed (counts per second) ---
        static long prevL = 0;
        static long prevR = 0;

        long dL = leftCount - prevL;
        long dR = rightCount - prevR;

        prevL = leftCount;
        prevR = rightCount;

        float leftCPS = dL / dt;
        float rightCPS = dR / dt;

        // --- Average speed for PID ---
        float avgSpeed = getWheelSpeed();

        // --- PID control ---
        float error = targetSpeed - avgSpeed;
        integral += error * dt;
        float derivative = (error - prevError) / dt;
        prevError = error;

        float output = Kp_rotate * error + Ki_rotate * integral + Kd_rotate * derivative;

        // Convert PID output to PWM
        int pwmBase = constrain(output, -255, 255);

        // Always rotate: left = +, right = -
        int pwmL = dir * abs(pwmBase);
        int pwmR = -dir * abs(pwmBase);

        setMotorL(pwmL);
        setMotorR(pwmR);

        Serial.print(leftCount);
        Serial.print(" ");
        Serial.println(rightCount);
    }

    // --- Stop motors ---
    setMotorL(0);
    setMotorR(0);
    delay(200);
}
