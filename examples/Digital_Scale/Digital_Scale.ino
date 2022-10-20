
#include "HX711.h"  //You must have this library in your arduino library folder

#define DOUT  11
#define CLK  13


HX711 scale;


//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 358640; //-358640 worked for 5Kg max scale setup 

//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(115200);
  
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare(); //Reset the scale to 0
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  scale.set_scale(calibration_factor);
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 3);
  Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.println();
}
//=============================================================================================
