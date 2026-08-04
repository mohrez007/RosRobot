 // ==========================================

// PIN DEFINITIONS

// ==========================================


// Motor A (Left Motor)

const int ENA = 9;   // PWM Speed Control

const int IN1 = 10;  // Direction Pin 1

const int IN2 = 11;  // Direction Pin 2


// Motor B (Right Motor)

const int ENB = 6;   // PWM Speed Control

const int IN3 = 7;   // Direction Pin 1

const int IN4 = 8;   // Direction Pin 2


// Encoder Pins (Motor A)

const int ENCODER_A = 3; 

const int ENCODER_B = 5;


// Encoder Pins (Motor B)

const int ENCODER_C = 2; 

const int ENCODER_D = 4;


// ==========================================

// ROBOT SPECIFICATIONS & MATH

// ==========================================

const float WHEEL_DIA_MM = 67.0;

const float GEAR_RATIO = 56.0;

const float ENCODER_PULSES = 11.0;


// Calculate pulses per mm

const float COUNTS_PER_REV = ENCODER_PULSES * GEAR_RATIO; // 616

const float WHEEL_CIRC = PI * WHEEL_DIA_MM;               // ~210.49 mm

const float COUNTS_PER_MM = COUNTS_PER_REV / WHEEL_CIRC;  // ~2.926 counts/mm


// ==========================================

// GLOBAL VARIABLES

// ==========================================

volatile long encoderTicksA = 0; 

volatile long encoderTicksB = 0; 


int baseSpeed = 200;             // Default PWM speed (0 to 255)

const float Kp = 1.5;            // Proportional Gain for motor sync


bool isMovingStraight = false;   // Flag to activate control loop

long targetTicks = 0;            // Used for specific distance driving

bool isDrivingDistance = false;


unsigned long lastReportTime = 0;

const unsigned long REPORT_INTERVAL = 100; 


// ==========================================

// INTERRUPT SERVICE ROUTINES (ISR)

// ==========================================

void readEncoderA() {

  if (digitalRead(ENCODER_B) == HIGH) {

    encoderTicksA--;

  } else {

    encoderTicksA++;

  }

}


void readEncoderC() {

  if (digitalRead(ENCODER_D) == HIGH) {

    encoderTicksB++;

  } else {

    encoderTicksB--;

  }  

}


// ==========================================

// SETUP

// ==========================================

void setup() {

  Serial.begin(115200);


  pinMode(ENA, OUTPUT);

  pinMode(IN1, OUTPUT);

  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);

  pinMode(IN3, OUTPUT);

  pinMode(IN4, OUTPUT);


  pinMode(ENCODER_A, INPUT);

  pinMode(ENCODER_B, INPUT);

  pinMode(ENCODER_C, INPUT);

  pinMode(ENCODER_D, INPUT);


  attachInterrupt(digitalPinToInterrupt(ENCODER_A), readEncoderA, RISING);

  attachInterrupt(digitalPinToInterrupt(ENCODER_C), readEncoderC, RISING);


  stopMotors();

}


// ==========================================

// MAIN LOOP

// ==========================================

void loop() {

  // 1. Process Incoming Serial Commands

  if (Serial.available() > 0) {

    char cmd = Serial.read();

    handleCommand(cmd);

  }


  // 2. Synchronize Motors if Moving Straight

  if (isMovingStraight) {

    synchronizeMotors();

  }


  // 3. Check Distance Goal if Active

  if (isDrivingDistance) {

    noInterrupts();

    long currentTicks = abs(encoderTicksA);

    interrupts();

    

    if (currentTicks >= targetTicks) {

      stopMotors();

      isDrivingDistance = false;

      Serial.println("Target Distance Reached.");

    }

  }


  // 4. Periodically send encoder ticks back over Serial

  if (millis() - lastReportTime >= REPORT_INTERVAL) {

    lastReportTime = millis();

    

    noInterrupts();

    long ticksA = encoderTicksA;

    long ticksB = encoderTicksB;

    interrupts();


    Serial.print("EA:");

    Serial.print(ticksA);

    Serial.print(" EB:");

    Serial.println(ticksB);

  }

}


// ==========================================

// CONTROL LOGIC

// ==========================================

void synchronizeMotors() {

  noInterrupts();

  long ticksA = abs(encoderTicksA);

  long ticksB = abs(encoderTicksB);

  interrupts();


  // Calculate the difference between left and right wheels

  long error = ticksA - ticksB; 

  

  // Adjust speeds based on the error

  // If error is positive, A is ahead (slow down A, speed up B)

  int speedA = baseSpeed - (error * Kp);

  int speedB = baseSpeed + (error * Kp);


  // Keep PWM values within valid 0-255 range

  speedA = constrain(speedA, 0, 255);

  speedB = constrain(speedB, 0, 255);


  analogWrite(ENA, speedA);

  analogWrite(ENB, speedB);

}


void driveDistance(float distance_mm) {

  targetTicks = distance_mm * COUNTS_PER_MM;

  isDrivingDistance = true;

  moveForward();

  Serial.print("Driving ");

  Serial.print(distance_mm);

  Serial.print("mm (Target Ticks: ");

  Serial.print(targetTicks);

  Serial.println(")");

}


// ==========================================

// MOTOR CONTROL FUNCTIONS

// ==========================================


void handleCommand(char cmd) {

  switch (cmd) {

    case 'w': 

    case 'W': moveForward(); break;

    case 's': 

    case 'S': moveBackward(); break;

    case 'a': 

    case 'A': turnLeft(); break;

    case 'd': 

    case 'D': turnRight(); break;

    case 'x': 

    case 'X': 

    case ' ': stopMotors(); break;

    case 'm': // Example specific distance trigger

    case 'M': driveDistance(500.0); break; // Drives 500mm

    case '+': 

      baseSpeed = min(255, baseSpeed + 15); 

      break;

    case '-': 

      baseSpeed = max(50, baseSpeed - 15); 

      break;

  }

}


void resetEncoders() {

  noInterrupts();

  encoderTicksA = 0;

  encoderTicksB = 0;

  interrupts();

}


void moveForward() {


  isMovingStraight = true;

  

  digitalWrite(IN1, HIGH);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);

  digitalWrite(IN4, LOW);

  

  // Initial kickoff speed, loop will take over sync immediately

  analogWrite(ENA, baseSpeed);

  analogWrite(ENB, baseSpeed);

}


void moveBackward() {


  isMovingStraight = true;

  

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, HIGH);

  

  analogWrite(ENA, baseSpeed);

  analogWrite(ENB, baseSpeed);

}


void turnLeft() {

  isMovingStraight = false; // Disable sync during turns

  isDrivingDistance = false;

  

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, HIGH);

  analogWrite(ENA, baseSpeed);


  digitalWrite(IN3, HIGH);

  digitalWrite(IN4, LOW);

  analogWrite(ENB, baseSpeed);

}


void turnRight() {

  isMovingStraight = false; // Disable sync during turns

  isDrivingDistance = false;

  

  digitalWrite(IN1, HIGH);

  digitalWrite(IN2, LOW);

  analogWrite(ENA, baseSpeed);


  digitalWrite(IN3, LOW);

  digitalWrite(IN4, HIGH);

  analogWrite(ENB, baseSpeed);

}


void stopMotors() {

  isMovingStraight = false;

  isDrivingDistance = false;

  

  analogWrite(ENA, 0);

  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);

  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);

  digitalWrite(IN4, LOW);

} 
