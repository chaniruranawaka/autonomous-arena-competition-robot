// --- Averaged analog read ---
int readAnalogSample(uint8_t pin) {
    long s = 0;
    for (uint8_t i = 0; i < SAMPLES; i++) {
        s += analogRead(pin);
        delayMicroseconds(80);
    }
    return s / SAMPLES;
}

// --- Initialize envelope ---
void initEnvelope() {
    for (uint8_t i = 0; i < N; i++) {
        minVal[i] = 4095;   // start assuming very high white
        maxVal[i] = 0;      // start assuming lowest black
    }
}

// --- Normalize (white → 0, black → 1) ---
float normalizeValue(uint8_t i, float raw) {
    float denom = maxVal[i] - minVal[i];
    if (denom <= 1) return 0;

    float v = (raw - minVal[i]) / denom;

    if (v <= 0.5) v = 0;
    if (v > 0.5) v = 1;

    return v;
}

// --- Calibration (for sensors where BLACK gives higher values) ---
void calibrateSensors(float norm[]) {

    Serial.println("=== Calibration Start ===");

    Serial.println("Step 1: Place robot over BLACK area!");
    digitalWrite(LED_G, HIGH);
    delay(3000);

    for (uint8_t i = 0; i < N; i++) {
        float v = readAnalogSample(analogPins[i]);
        if (v > maxVal[i]) maxVal[i] = v;  // black (high)
    } 

    digitalWrite(LED_G, LOW);

    Serial.println("Step 2: Place robot over WHITE area!");
    delay(3000);

    for (uint8_t i = 0; i < N; i++) {
        float v = readAnalogSample(analogPins[i]);
        if (v < minVal[i]) minVal[i] = v;  // white (low)
    }
    Serial.println("=== Calibration Done ===");

}
