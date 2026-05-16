void grab_ball(){

  driveServo(0, 180); // gripper opening

  delay(1000);

  driveServo(180, 0); //gripper closing

  delay(1000);

  driveServoSlow(5, 120);   // move ankle upwards

  delay(2000);

  driveServoSlow(120, 5); // move ankle downwards

  delay(1000);

  driveServo(0, 180); // gripper opening
}

void grab_box(){

  driveServo(0, 180); // gripper opening

  delay(1000);
  
  driveServo(180, 60); //gripper closing

  delay(1000);

  driveServoSlow(0, 120);   // move ankle upwards

  delay(2000);

  driveServoSlow(120, 0); // move ankle downwards

  delay(1000);

  driveServo(60, 180); // gripper opening
}

void driveServo(int start, int end) {
  if (end > start){
    for (int angle = start; angle <= end; angle += 2) {
      gripper.write(angle);
      delay(15);
    }
    delay(300);
  }

  else{
    for (int angle = start; angle >= end; angle -= 2) {
      gripper.write(angle);
      delay(15);
    }
    delay(300);
  }
  
}

void driveServoSlow(int start, int end) {

  if (end > start){
    for (int angle = start; angle <= end; angle += 1) {
      ankle.write(angle);
      delay(25);
    }
    delay(500);
  }

  else{
    for (int angle = start; angle >= end; angle -= 1) {
      ankle.write(angle);
      delay(25);
    }
    delay(500);
  }
}


