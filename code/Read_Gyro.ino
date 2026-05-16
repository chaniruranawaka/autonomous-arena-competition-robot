float read_angle() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;

  float angleX = atan2(ax, az) * 180.0 / PI;   // X-axis tilt
  float angleY = atan2(ay, az) * 180.0 / PI;   // Y-axis tilt

  return angleX;
  delay(50);
}
