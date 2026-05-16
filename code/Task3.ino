int detectSharpTurn() {
    // Check if the robot is approaching a sharp turn (90-degree turn)
    bool leftLostLine = (norm[0] == 0 && norm[1] == 0 && norm[2] == 0);  // Left sensors lost the line
    bool rightLostLine = (norm[6] == 0 && norm[7] == 0 && norm[5] == 0); // Right sensors lost the line

    if (leftLostLine || rightLostLine) {
        // Turn detected, now we need to figure out if it's a left or right turn
        if (leftLostLine) {
            return -1; // -1 if right turn
            // turnRight(); // Perform a right turn
        } else if (rightLostLine) {
            return 1; // 1 if left turn
            // turnLeft(); // Perform a left turn
        }
    }
    return 0; // No sharp turn detected
}

void brake() {
    setMotorL(0);
    setMotorR(0);
}

bool barcode_detected = false;

bool detectbarcode() {
    if (!barcode_detected) {
        for (int i = 0; i < 8; i++) {
            if (norm[i] == 1) {  // If any sensor reads black (1)
                return false;    // Return false immediately if black is detected
            }
        }
        barcode_detected = true;
        return true;  // Return true if all sensors read white (0)
    }
    return false;
}

bool detectcross() {

    for (int i = 0; i < 8; i++) {
        if (norm[i] == 1) {  // If any sensor reads black (1)
            return false;    // Return false immediately if black is detected
        }
    }
    return true;  // Return true if all sensors read white (0)
}

void task3() {
    while (detectSharpTurn() == 0) {
        line_following();  // Continue line following

        if (detectbarcode()) {
            read_barcode();  // Read and process the barcode

        if (barcode_detected && detectcross()){
          brake();
          rotate_angle(-90,100);
          line_following(); 
          // follow the line untill the wall is detected

        }
        }
    }

    brake();  // Stop robot movement

    if (detectSharpTurn() == 1) { 
        rotate_angle(90, 100);  // Turn left 90 degrees
    } else if (detectSharpTurn() == -1) {
        rotate_angle(-90, 100);  // Turn right 90 degrees
    }
}
