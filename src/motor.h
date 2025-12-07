#include <Arduino.h>

class Stepper{
  public:
    int DIR_PIN;
    int STEP_PIN;
    const int stepsPerRevolution = 200;
    const float stepsPerDeg = (stepsPerRevolution) / 360.0;
    const int maxIdx = 4;
    const int maxDeg = 180;

    int currentAngle = 0;

    Stepper(int dir, int step){
      DIR_PIN = dir;
      STEP_PIN = step;
      motorInit();
    }

    void motorInit() {
      pinMode(STEP_PIN, OUTPUT);
      pinMode(DIR_PIN, OUTPUT);
    }

    void moveMotor(int steps) {
      digitalWrite(DIR_PIN, (steps > 0));

      if (steps < 0) steps = -steps;   // always step positive count

      for (int i = 0; i < steps; i++) {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(3000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(3000);
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

    void moveClockwise(int n) {
      moveMotor(stepsPerRevolution * n);
    }
};