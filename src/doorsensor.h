#include <Arduino.h>

class DoorSensor{
    public:
        int TRIG = 22;
        int ECHO = 23;

        DoorSensor(){
            initDoorSensor();
        }
        DoorSensor(int trig, int echo){
            TRIG = trig;
            ECHO = echo;
            initDoorSensor();
        }
        void initDoorSensor(){
            pinMode(TRIG, OUTPUT);
            pinMode(ECHO, INPUT);
        }
        float getDistance(){
            digitalWrite(TRIG, LOW);
            delayMicroseconds(2);
            digitalWrite(TRIG, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG, LOW);
            
            long duration = pulseIn(ECHO, HIGH);
            float distance = (duration * 0.034) / 2;
            return distance;
        }
};