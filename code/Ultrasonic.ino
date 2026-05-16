long readUltrasonicDistance2() {
  // Send a 10us pulse to trigger pin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Read the echo pin duration (in microseconds)
  long duration = pulseIn(echoPin2, HIGH);

  // Convert duration to distance in cm
  long distance = duration * 0.34 / 2; // speed of sound = 0.034 cm/us

  return distance;
}

long readUltrasonicDistance1() {
  // Send a 10us pulse to trigger pin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Read the echo pin duration (in microseconds)
  long duration = pulseIn(echoPin1, HIGH);

  // Convert duration to distance in cm
  long distance = duration * 0.34 / 2; // speed of sound = 0.034 cm/us

  return distance;
}


