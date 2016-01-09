//Load Cell Testing
//Reads values from load cells
//Using HX711 load cell amps
//Requires bodge's HX711 library

#include <HX711.h>
int CLK = 49; //Clock pin for HX711
float calibration_factor = -7050.0; //This value is obtained using the SparkFun_HX711_Calibration sketch

HX711 scale(11, CLK);
HX711 scale2(10, CLK);
HX711 scale3(9, CLK);
HX711 scale4(8, CLK);

void setup() {
  Serial.begin(9600);
  scale.set_scale(calibration_factor); //Calibrate load cells
  scale2.set_scale(calibration_factor);
  scale3.set_scale(calibration_factor);
  scale4.set_scale(calibration_factor);
}

void loop() {
  Serial.print("Values: ");
  Serial.print(scale.get_units(), 1);
  Serial.print(", ");
  Serial.print(scale2.get_units(), 1);
  Serial.print(", ");
  Serial.print(scale3.get_units(), 1);
  Serial.print(", ");
  Serial.print(scale4.get_units(), 1);
  Serial.println();
}
