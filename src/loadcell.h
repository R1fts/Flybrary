#include <Arduino.h>
#include "HX711.h"

class LoadCell{
  public:
    int LOADCELL_DOUT_PIN = 26;
    int LOADCELL_SCK_PIN = 27;
    HX711 scale;

    LoadCell(){
      loadCellInit();
    }
    LoadCell(int dout, int sck){
      LOADCELL_DOUT_PIN = dout;
      LOADCELL_SCK_PIN = sck;
      loadCellInit();
    }
    void loadCellInit() {
      Serial.begin(115200);
      scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    }

    void calibrate() {
      if (scale.is_ready()) {
        scale.set_scale();    
        Serial.println("Tare... remove any weights from the scale.");
        delay(5000);
        scale.tare();
        Serial.println("Tare done...");
        Serial.print("Place a known weight on the scale...");
        delay(5000);
        long reading = scale.get_units(10);
        Serial.print("Result: ");
        Serial.println(reading);
      } 
      else {
        Serial.println("HX711 not found.");
      }
      delay(1000);  
    }

    long detectWeight(){
      if (scale.is_ready()) {
        long weight = scale.get_units(10);
        return weight;
      } else {
        Serial.println("HX711 not found.");
        return -1;
      }
    }

    bool detect(){
      if(detectWeight() >= 100) return true;
      else return false;
    }
};
//calibration factor will be the (reading)/(known weight)