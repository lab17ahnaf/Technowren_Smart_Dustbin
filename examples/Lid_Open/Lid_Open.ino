#include <Servo.h>
#include <HCSR04.h>

//-----------------------------------------------------------------------
/* Sonar Config */
#define HSSR04_Trig 2
#define HSSR04_Echo 3
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
/* Servo Config */
#define Servo_Attach 10
//-----------------------------------------------------------------------

HCSR04 hcsr04(HSSR04_Trig, HSSR04_Echo);
Servo servo;

//-----------------------------------------------------------------------
/* 'Auto lid open' Config */
const int Lid_Opening_Distance = 50; // In cm
const int Lid_Halt_Duration = 5000;  // In ms
const int Lid_Halt_Offset = 4000;    // In ms
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
/* 'Auto lid open' global variables */
float instantaneous_distance = 250;
long clock_lidOpen = 0;
bool flag = true;
bool lid_open = false;
//-----------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);

  //-----------------------------------------------------------------------
  /* Servo setup */
  servo.attach(Servo_Attach);
  servo.write(0);
  delay(1000);
  //-----------------------------------------------------------------------
}

void loop()
{
  if (millis() - clock_lidOpen > Lid_Halt_Duration)
  {
    instantaneous_distance = int(hcsr04.dist());
    flag = true;
  }

  Serial.println(instantaneous_distance);

  if (instantaneous_distance < Lid_Opening_Distance)
  {
    if (flag == true)
    {
      servo.write(90);
      lid_open = true;
      clock_lidOpen = millis();
      flag = false;
    }
  }
  else if (lid_open)
  {
    clock_lidOpen = millis() + Lid_Halt_Offset;
    servo.write(0);
    lid_open = false;
  }
}
