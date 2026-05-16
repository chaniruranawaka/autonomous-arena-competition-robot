int X = 0;
int Y = 0;
int cross = 0;
int dir = 1;
bool obstacle = false;

void count_cross() {
  // your count_cross logic here
  bool allWhite = true;

  for (int i = 0; i < 8; i++) {
    if (norm[i] != 0) allWhite = false;
  }

  if (allWhite) {       // FIXED capitalization
    cross += 1;         // FIXED missing semicolon
  }
}

bool detect_obstacle(){
  // If both TOF sensors give close readings less than 10 cm then return true, false otherwise 
  
}

bool detect_box(){
  // If both TOF sensors give close readings less than 10 cm then return true, false otherwise 
  
}

void go_section_L() {
  // ------------ FIRST SECTION ---------------
  while (cross <= 6 && !obstacle) {
    line_following();
    obstacle = detect_obstacle();
    count_cross();
  }

  if (obstacle){
    // execute the obstacle avoidance algorithm
  }
  
  if (cross == 7) {
    setMotorL(0);        // FIXED function name (setmotorL → setMotorL)
    setMotorR(0);
    cross = 0;           // reset counter
    rotate_angle(90 * dir, 100);   // turn 90° left or right
  }

  // ------------ SECOND SECTION ---------------
  while (cross <= 1) {
    line_following();
    count_cross();
  }

  if (cross == 2) {
    setMotorL(0);
    setMotorR(0);
    cross = 0;
    rotate_angle(90 * dir, 100);

    dir *= -1;   // FIXED missing semicolon
  }
}
