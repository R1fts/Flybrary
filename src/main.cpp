#include <Arduino.h>
#include "motor.h"
#include "loadcell.h"
#include "servo.h"

void setup() {
  motorInit();
  loadCellInit();
  servoInit();
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    int value = Serial.parseInt();
    write(value);
  }
}