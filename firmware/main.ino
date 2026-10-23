#include <Servo.h>

const int PIN_IR_LEFT = A0;
const int PIN_IR_CENTER = A1;
const int PIN_IR_RIGHT = A2;
const int PIN_LDR = A4;

const int PIN_TRIG = 2;
const int PIN_ECHO = 4;
const int PIN_SERVO = 3;

const int PIN_ENA = 5;
const int PIN_IN1 = 6;
const int PIN_IN2 = 7;
const int PIN_IN3 = 8;
const int PIN_IN4 = 9;
const int PIN_ENB = 10;

const int PIN_SPEAKER = 11;
const int PIN_BT_STATE = 12;

Servo obstacleServo;

const int IR_THRESHOLD = 500;
const int LDR_THRESHOLD = 400;
const int SAFE_DISTANCE = 25;
const int BASE_SPEED = 180;
const float Kp = 0.5;

void setMotors(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    leftSpeed = -leftSpeed;
  }
  
  if (rightSpeed >= 0) {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  } else {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    rightSpeed = -rightSpeed;
  }

  analogWrite(PIN_ENA, constrain(leftSpeed, 0, 255));
  analogWrite(PIN_ENB, constrain(rightSpeed, 0, 255));
}

void stopMotors() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}

long getDistance() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long duration = pulseIn(PIN_ECHO, HIGH, 25000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

int scanBestDirection() {
  stopMotors();
  
  obstacleServo.write(30);
  delay(200);
  long rightDist = getDistance();
  
  obstacleServo.write(150);
  delay(200);
  long leftDist = getDistance();
  
  obstacleServo.write(90);
  delay(150);

  if (leftDist < SAFE_DISTANCE && rightDist < SAFE_DISTANCE) {
    return 0;
  } else if (leftDist > rightDist) {
    return 1;
  } else {
    return 2;
  }
}

void executeEvasion() {
  tone(PIN_SPEAKER, 300, 150);
  setMotors(-150, -150);
  delay(400);
  
  int bestDir = scanBestDirection();
  if (bestDir == 1) {
    setMotors(-160, 160);
    delay(350);
  } else if (bestDir == 2) {
    setMotors(160, -160);
    delay(350);
  } else {
    setMotors(-180, 180);
    delay(600);
  }
  stopMotors();
}

void playKineticSynth(int leftVal, int centerVal, int rightVal, int ldrVal) {
  int netSway = (rightVal - leftVal);
  int pitch = map(netSway, -800, 800, 150, 1200);
  pitch = constrain(pitch, 120, 1500);

  if (ldrVal < LDR_THRESHOLD) {
    tone(PIN_SPEAKER, pitch * 2, 40);
  } else if (centerVal < IR_THRESHOLD || leftVal < IR_THRESHOLD || rightVal < IR_THRESHOLD) {
    tone(PIN_SPEAKER, pitch, 30);
  } else {
    noTone(PIN_SPEAKER);
  }
}

void setup() {
  pinMode(PIN_IR_LEFT, INPUT);
  pinMode(PIN_IR_CENTER, INPUT);
  pinMode(PIN_IR_RIGHT, INPUT);
  pinMode(PIN_LDR, INPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_BT_STATE, INPUT);

  obstacleServo.attach(PIN_SERVO);
  obstacleServo.write(90);

  stopMotors();
}

void loop() {
  if (getDistance() < SAFE_DISTANCE) {
    executeEvasion();
    return;
  }

  int leftIR = analogRead(PIN_IR_LEFT);
  int centerIR = analogRead(PIN_IR_CENTER);
  int rightIR = analogRead(PIN_IR_RIGHT);
  int ldrVal = analogRead(PIN_LDR);

  bool btConnected = (digitalRead(PIN_BT_STATE) == HIGH);

  if (!btConnected) {
    playKineticSynth(leftIR, centerIR, rightIR, ldrVal);
  } else {
    noTone(PIN_SPEAKER);
  }

  bool laserDetected = (ldrVal < LDR_THRESHOLD);
  bool leftActive = (leftIR < IR_THRESHOLD);
  bool centerActive = (centerIR < IR_THRESHOLD);
  bool rightActive = (rightIR < IR_THRESHOLD);

  if (laserDetected) {
    setMotors(BASE_SPEED + 20, BASE_SPEED + 20);
  } else if (centerActive && !leftActive && !rightActive) {
    setMotors(BASE_SPEED, BASE_SPEED);
  } else if (leftActive || rightActive || centerActive) {
    int error = (rightIR - leftIR);
    int turnOffset = (int)(error * Kp);

    int leftMotorSpeed = BASE_SPEED + turnOffset;
    int rightMotorSpeed = BASE_SPEED - turnOffset;

    setMotors(leftMotorSpeed, rightMotorSpeed);
  } else {
    stopMotors();
  }

  delay(20);
}
