#include <Arduino.h>
#include "motor.h"

void setup() {
  motorInit();
}

void loop() {
    moveToIdx(0);
    delay(2000);
    moveToIdx(1);
    delay(2000);
    moveToIdx(2);
    delay(2000);
    moveToIdx(3);
    delay(2000);
    moveToIdx(4);
    delay(2000);
    moveToIdx(5);
    delay(2000);
    moveToIdx(6);
    delay(2000);
    moveToIdx(7);
    delay(2000);
    moveToIdx(8);
    delay(2000);
    moveToIdx(9);
    delay(5000);
}