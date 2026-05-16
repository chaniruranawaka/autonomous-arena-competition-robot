float measureBarLength(bool targetWhite) {

    bool barStarted = false;
    float barLength = 0;

    // Reset encoder at start
    leftCount = 0;
    rightCount = 0;

    while (true) {
        // Determine current bar color
        bool allWhite = true;
        for (int i = 0; i < 8; i++) {
            if (norm[i] != 0) allWhite = false;
        }

        bool currentBarIsWhite = allWhite;

        // Start of target bar
        if (currentBarIsWhite == targetWhite && !barStarted) {
            barStarted = true;
            leftCount = 0;
            rightCount = 0;
        }

        // End of target bar
        if (currentBarIsWhite != targetWhite && barStarted) {
            float avgCounts = (abs(leftCount) + abs(rightCount)) / 2.0;
            barLength = (avgCounts / CPR) * wheelCirc;
            return barLength;
        }

        // Keep robot moving forward (insert your PID / motor code)
        move_straight(0.01);
    }
}



void read_barcode() {

    for (int i = 0; i < 6; i++) {
        // Measure the next white bar
        float barLength = measureBarLength(true);  // waits until white bar is detected

        // Simple threshold to classify bar as 1 or 0
        if (barLength > 0.045) {   // 4.5 cm → adjust according to your robot
            barcode[i] = 1;
        } else {
            barcode[i] = 0;
        }
    }
}










