#include <Arduino.h>

const int TRIG = 22;
const int ECHO = 23;
void initDoorSensor(){
    pinMode(TRIG, INPUT);
    pinMode(TRIG, OUTPUT);
}