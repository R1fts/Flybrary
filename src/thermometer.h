#include <Arduino.h>

class Thermo{
    public:
        int ANALOG_PIN;
        float beta;
        const float R0   = 10000;
        const float BETA = 3950;
        const float T0   = 298.15;
        
        Thermo(int pin){
            ANALOG_PIN = pin;
        }
        float readTempCelsius() {
            float adc = analogRead(ANALOG_PIN);
            return 55.3333333 - (adc / 15.0);
        }
};