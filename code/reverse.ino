void move_reverse(float targetMeters) {

    leftCount = 0;
    rightCount = 0;
    lastTime_move_straight = millis();

    float prevLeft = 0;
    float prevRight = 0;
    float distanceTravelled = 0;

    float integral = 0;

    float targetSpeedRev = -abs(targetSpeed);   // reverse speed

    while (distanceTravelled < (targetMeters - 0.02)) {

        unsigned long now = millis();
        float dt = (now - lastTime_move_straight) / 1000.0;
        if (dt <= 0) continue;
        lastTime_move_straight = now;

        // Wheel delta
        float dLeft = leftCount - prevLeft;
        float dRight = rightCount - prevRight;

        prevLeft = leftCount;
        prevRight = rightCount;

        float vLeft = (dLeft / CPR) * wheelCirc / dt;
        float vRight = (dRight / CPR) * wheelCirc / dt;

        float vAvg = (vLeft + vRight) / 2.0;

        // Speed PID
        float error = targetSpeedRev - vAvg;
        integral += error * dt;
        float output = Kp_speed * error + Ki_speed * integral;

        int pwmBase = constrain(output, -210, -60); 
        
        // Straight-line correction (same formula)
        float diff = dLeft - dRight;
        int correction = diff * 1.5;

        int pwml = pwmBase - correction;      
        int pwmr = pwmBase + correction;

        pwmr = constrain(pwmr, -210, -60);
        pwml = constrain(pwml, -210, -60);

        setMotorL(pwml);
        setMotorR(pwmr);

        // Distance (forward or reverse both use abs)
        float avgCounts = (abs(leftCount) + abs(rightCount)) / 2.0;
        distanceTravelled = (avgCounts / CPR) * wheelCirc;
        Serial.print(leftCount);
        Serial.print("  ");
        Serial.println(rightCount);
    }

    brake();
}
