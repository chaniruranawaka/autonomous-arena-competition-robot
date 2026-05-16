void move_straight(float targetMeters) {

    calibrateMotorDrift();

    // Clear state
    leftCount = 0;
    rightCount = 0;
    lastTime_move_straight = millis();

    float prevLeft = 0;
    float prevRight = 0;
    float distanceTravelled = 0;

    float integral = 0;
    float prevError = 0;

    while (distanceTravelled < (targetMeters - 0.02)) {

        unsigned long now = millis();
        float dt = (now - lastTime_move_straight) / 1000.0;
        if (dt <= 0) continue;
        lastTime_move_straight = now;

        float dLeft = leftCount - prevLeft;
        float dRight = rightCount - prevRight;

        prevLeft = leftCount;
        prevRight = rightCount;

        float vLeft = (dLeft / CPR) * wheelCirc / dt;
        float vRight = (dRight / CPR) * wheelCirc / dt;

        float vAvg = (vLeft + vRight) / 2.0;

        float error = targetSpeed - vAvg;
        integral += error * dt;
        float output = Kp_speed * error + Ki_speed * integral;

        // PWM base level
        int pwmBase = constrain(output, 60, 210);

        float diff = dLeft - dRight;
        int correction = diff * 2;  

        // For correct the encoder drift
        int pwml = pwmBase - correction;
        int pwmL1 = pwml * motorTrim; 

        int pwmr = pwmBase + correction;


        int pwmL = constrain(pwmL1, 60, 210);
        int pwmR = constrain(pwmr, 60, 210);

        setMotorL(pwmL);
        setMotorR(pwmR);

        float avgCounts = (abs(leftCount) + abs(rightCount)) / 2.0;
        distanceTravelled = (avgCounts / CPR) * wheelCirc;
    }

    // STOP
    brake();
}
