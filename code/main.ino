  #include <Wire.h>
  #include <Servo.h>
  #include <Adafruit_VL53L0X.h>
  #include <Adafruit_MPU6050.h>
  #include <Adafruit_Sensor.h>
  #include <math.h>

Servo gripper;   // MG90S on pin 4
Servo ankle;     // MG995 on pin 11

Adafruit_MPU6050 mpu;

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
#define XSHUT1 34


// === Calibration Variables ===
const uint8_t N = 8;
const uint8_t analogPins[N] = {A8, A9, A10, A11, A12, A13, A14, A15};

float minVal[N];
float maxVal[N];
float norm[N];   // <-- normalized values array
float barcode[6]; // to store the barcode


const uint8_t SAMPLES = 3;

#define IN1 7
#define IN2 8
#define ENA 5
#define IN3 9
#define IN4 10
#define ENB 6

#define LED_G 51
#define LED_R 52

const int encA_L = 2;
const int encB_L = 3;
const int encA_R = 18;
const int encB_R = 19;

// PID for line following
float Kp = 70.0;
float Ki = 0.0;
float Kd = 70.0;

float error = 0;
float lastError = 0;
float integral = 0;

// PID for speed control
float Kp_speed = 1000;
float Ki_speed = 10;
float Kd_speed = 15;

// PID for circle
float Kp_circle = 0.05;
float Ki_circle = 0;
float Kd_circle = 0.001;


// Motor PWM limits
int baseSpeed = 80;
int maxPWM = 180;

// Encoder counts
volatile long leftCount = 0;
volatile long rightCount = 0;

// Desired wheel speed (counts per loop)
int targetLeftSpeed = 30;
int targetRightSpeed = 30;

// === Ramp climbing variables ===

//Robot Params
float wheelRadius = 0.032;    // meters
int CPR = 450;
const float wheelCirc = 2.0 * PI * wheelRadius;
const float wheelBase = 0.2;


//Speed Control
float targetSpeed = 0.30;     // m/s
float currentSpeed = 0;       // filtered smooth speed
float accLimit = 0.02;        // m/s^2 each loop update

//Ramp Control
float pitch = 0;
float rampUpBoost = 0.10;     // speed boost during incline
float pitchThreshold_climb = -5;     // degrees
float pitchThreshold_descend = 5;
float kRamp = 20;             // feedforward gain
float rampDownSlow = 0.25;          // speed reduction when descending
float minDownhillSpeed = 0.08;      // don't go too slow or robot becomes unstable
float kDown = 15;                   // braking force constant


//Timing
unsigned long lastTime_speed = 0;
unsigned long lastTime_move_straight = 0;


// TOF and Ultrasonic
int tofdistance = 0;
const int trigPin1 = 46;
const int echoPin1 = 47;
const int trigPin2 = 49;
const int echoPin2 = 48;


// Encoder calibration
float motorTrim = 0.92;   // 1.0 = no correction

// -- Grid Solving Variables ---
unsigned long crossCooldown = 500;
unsigned long lastCrossTime = 0;

bool crossDetected = false;


void setup() {
    Serial.begin(115200);
    Wire.begin();

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(encA_L, INPUT_PULLUP);
    pinMode(encB_L, INPUT_PULLUP);
    pinMode(encA_R, INPUT_PULLUP);
    pinMode(encB_R, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(encA_L), leftEncoderA, RISING);
    attachInterrupt(digitalPinToInterrupt(encB_L), leftEncoderB, RISING);

    attachInterrupt(digitalPinToInterrupt(encA_R), rightEncoderA, RISING);
    attachInterrupt(digitalPinToInterrupt(encB_R), rightEncoderB, RISING);

    gripper.attach(4);
    ankle.attach(11);
  
    // Indicater LEDs
    pinMode(LED_G, OUTPUT);
    pinMode(LED_R, OUTPUT);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_R, LOW);

    pinMode(XSHUT1, OUTPUT);
    digitalWrite(XSHUT1, LOW);
    delay (10);
    digitalWrite(XSHUT1, HIGH);             

    // Ultrasonic 
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);

    // for switches
    pinMode(36, INPUT_PULLUP);
    pinMode(37, INPUT_PULLUP);
    pinMode(38, INPUT_PULLUP);
    pinMode(39, INPUT_PULLUP);
    pinMode(40, INPUT_PULLUP);
    pinMode(41, INPUT_PULLUP);
    pinMode(42, INPUT_PULLUP);
    pinMode(43, INPUT_PULLUP);

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    if (digitalRead(36) == LOW) {
      initEnvelope();
      calibrateSensors(norm);  // WHITE - 0   BLACK - 1
    }
    
}

void loop() {
    if (digitalRead(37) == LOW) {
      task1();
    }
    
    else if (digitalRead(38) == LOW) {
      task2();
    }

    else if (digitalRead(39) == LOW) {
      task3();
    }
}
