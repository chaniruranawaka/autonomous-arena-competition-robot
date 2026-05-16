int tof(){
  VL53L0X_RangingMeasurementData_t measure;

  lox1.rangingTest(&measure, false); // false = no debug output
  
  int d1 = (measure.RangeStatus !=4 ) ? measure.RangeMilliMeter : -1 ;
  Serial.println(d1);
  return d1;

  delay(100);
}