#include <SoftwareSerial.h>

SoftwareSerial gsm(7,10); // RX, TX
void setup(){
  Serial.begin(9600);
  gsm.begin(9600);
  gsm.write("AT");
}

void loop()
{
  
  while (Serial.available())  // forward data from monitor to esp
    gsm.write(Serial.read());
  while (gsm.available())  // forward data from esp to monitor
    Serial.write(gsm.read());
}
