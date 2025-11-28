#include <Arduino.h>
#include <ESP32Servo.h>

//define servo pins
const int servo1 = 12;
const int servo2 = 13;

Servo s1;
Servo s2;

void servoInit() {
  s1.attach(servo1);
  s2.attach(servo2);
}

// Move both servos to 90 degrees
void openServo() {
  s1.write(0);
  s2.write(0);
  delay(300); 
  s1.write(90);
  s2.write(90);
}

// Move both servos back to 0 degrees
void closeServo() {
  s1.write(180);
  s2.write(180);
  delay(300); 
  s1.write(90);
  s2.write(90);
}

void write(int n){
  s1.write(n);
}