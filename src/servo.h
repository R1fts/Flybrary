#include <Arduino.h>
#include <ESP32Servo.h>

//define servo pins
class BorrowServo {
  public:
    int SERVO_PIN = 12;
    Servo s1;

    BorrowServo(){
      servoInit();
    }
    BorrowServo(int s){
      SERVO_PIN = s;
      servoInit();
    }
    void servoInit() {
      s1.attach(SERVO_PIN);
    }

    // Move both servos to 90 degrees
    void open() {
      s1.write(0);
      delay(300); 
      s1.write(90);
    }

    // Move both servos back to 0 degrees
    void close() {
      s1.write(180);
      delay(300); 
      s1.write(90);
    }

    void write(int n){
      s1.write(n);
    }
};