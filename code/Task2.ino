int dashed_count = 0;
bool dashActive = true;

void follow_dashed_line() {
    float oldkp = Kp;
    float oldki = Ki;
    float oldkd = Kd;
    float oldspeed = baseSpeed;

    Kp = 120.0;
    Ki = 0.0;
    Kd = 90.0;
    baseSpeed = 70;

    while (!detectWhite()) {
        line_following();
    } // This was missing in the original code

    if (detectWhite()){
      setMotorL(0);
      setMotorR(0);
      move_straight(0.5);
      move_reverse(0.1);
      rotate_angle(180,100);


    }
    while (!detectWhite()) {
        move_straight(0.01);
       
    } 

    if (detectWhite()){
      while (dashed_count <= 15){
        line_following();
        count_dash();
      }
      
    }

    rotate_angle(90,100);
    // climb the ramp



  }

void count_dash() {
    bool anyLine = false;  // Will be true if any sensor sees white (0)

    
    // Check sensor readings
    for (int i = 0; i < 8; i++) {
        if (norm[i] == 0) {  // If any sensor sees white (0)
            anyLine = true;   // The robot is on the white line
        }
    }

    if (!dashActive && !anyLine) {
        dashed_count += 1;  // Increment dashed count
        dashActive = true;  // Set dashActive to true to prevent counting the same line multiple times
    }

    if (anyLine) {
        dashActive = false;  // Allow counting the next dashed line when robot leaves the white line
    }
}

bool detectWhite() {

    for (uint8_t i = 0; i < N; i++) {
    float v = readAnalogSample(analogPins[i]);  // Read the sensor value
    norm[i] = normalizeValue(i, v);  // Normalize and store in norm[]
  }

    for (int i = 0; i < 8; i++) {
        if (norm[i] == 1) {  // If any sensor reads black (1)
            return false;    // Return false immediately if black is detected
        }
    }
    return true;  // Return true if all sensors read white (0)
}
