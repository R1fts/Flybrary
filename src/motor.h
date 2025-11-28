#include <Arduino.h>

// Define pin connections & motor's steps per revolution
const int dirPin = 19;
const int stepPin = 18;

// REAL steps of the motor
const int stepsPerRevolution = 200;
const float stepsPerDeg = (stepsPerRevolution) / 360.0;

// Max number of books and degree of arch
const int maxIdx = 9;
const int maxDeg = 180;

int currentAngle = 0;

void motorInit() {
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void moveMotor(int steps) {
  digitalWrite(dirPin, (steps > 0));

  if (steps < 0) steps = -steps;   // always step positive count

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}

void moveToIdx(int idx) {
  if (idx < 0 || idx > maxIdx) return;

  // Map idx 0–9 → angle 0–180
  int targetAngle = map(idx, 0, maxIdx, 0, maxDeg);
  // Calculate required steps
  int deltaAngle = targetAngle - currentAngle;
  int stepCount = deltaAngle * stepsPerDeg;
  
  // Move stepper
  moveMotor(stepCount);

  // Update angle state
  currentAngle = targetAngle;
}