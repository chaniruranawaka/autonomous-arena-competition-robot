void move_circle(float radius_m, float angle_deg)
{
    float angle_rad = angle_deg * PI / 180.0;

    float arc_len = radius_m * angle_rad;

    float R_left  = radius_m - (wheelBase / 2.0);
    float R_right = radius_m + (wheelBase / 2.0);

    float ratio_L = R_left  / radius_m;
    float ratio_R = R_right / radius_m;

    int pwmL_target = constrain(baseSpeed * ratio_L, 60, 255);
    int pwmR_target = constrain(baseSpeed * ratio_R, 60, 255);

    leftCount = 0;
    rightCount = 0;

    float distanceTravelled = 0;
    unsigned long lastTime = millis();

    // PID memory
    float iL = 0, iR = 0;
    float prevEL = 0, prevER = 0;
    long prevLeft = 0, prevRight = 0;

    while (distanceTravelled < arc_len) {

        unsigned long now = millis();
        float dt = (now - lastTime) / 1000.0;
        if (dt <= 0) continue;
        lastTime = now;

        long dL = leftCount - prevLeft;
        long dR = rightCount - prevRight;

        prevLeft = leftCount;
        prevRight = rightCount;

        float vL = ((float)dL / CPR) * wheelCirc / dt;
        float vR = ((float)dR / CPR) * wheelCirc / dt;


        float targetL = pwmL_target;  // using PWM as a speed proxy
        float targetR = pwmR_target;

        float eL = targetL - vL;
        float eR = targetR - vR;

        iL += eL * dt;
        iR += eR * dt;

        // --- NEW: Derivative terms ---
        float dEL = (eL - prevEL) / dt;
        float dER = (eR - prevER) / dt;

        prevEL = eL;
        prevER = eR;

        int pwmL = pwmL_target + (Kp_circle * eL + Ki_circle * iL + Kd_circle * dEL);
        int pwmR = pwmR_target + (Kp_circle * eR + Ki_circle * iR + Kd_circle * dER);

        pwmL = constrain(pwmL, 60, 255);
        pwmR = constrain(pwmR, 60, 255);

        setMotorL(pwmL);
        setMotorR(pwmR);

        Serial.print(pwmL);
        Serial.print(" ");
        Serial.println(pwmR);


        float avgCounts = (abs(leftCount) + abs(rightCount)) * 0.5;
        distanceTravelled = (avgCounts / CPR) * wheelCirc;
    }

    setMotorL(0);
    setMotorR(0);
}
