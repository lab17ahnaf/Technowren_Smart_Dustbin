#include <Arduino.h>
#include <_config.h>

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
/*_____________________________ Auto Lid Open config & declarations _____________________________________*/
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Modem selection: (Sim800l is currently in use)
#define TINY_GSM_MODEM_SIM800
//-----------------------------------------------------------------------

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

//-----------------------------------------------------------------------
// Blynk App Authentication Token.
char auth[] = BLYNK_AUTH_TOKEN;
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// GPRS credentials
char apn[] = NETWORK_APN;
char user[] = NETWORK_USER;
char pass[] = NETWORK_PASS;
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1
//-----------------------------------------------------------------------
// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); // RX, TX
TinyGsm modem(SerialAT);

//-----------------------------------------------------------------------
// Object declaration
BlynkTimer timer;
#define INTERVAL 1000L

int value = 55;

/************************************************************************************
 *  This function sends Arduino's up time every second to Virtual Pin.
 *  In the app, Widget's reading frequency should be set to PUSH. This means
 *  that you define how often to send data to Blynk App.
 **********************************************************************************/
void SendDhtData()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, value);
  //-----------------------------------------------------------------------
}

void setup()
{
  //-----------------------------------------------------------
  // Debug console
  Serial.begin(115200);
  delay(10);
  //-----------------------------------------------------------
  // Set GSM module baud rate
  SerialAT.begin(9600);
  // SerialAT.begin(9600,SWSERIAL_8N1,D3,D4);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // Unlock your SIM card with a PIN
  // modem.simUnlock("1234");
  //-----------------------------------------------------------
  Blynk.begin(auth, modem, apn, user, pass);
  //-----------------------------------------------------------
  // Setup a function to be called every second
  timer.setInterval(INTERVAL, SendDhtData);
  //-----------------------------------------------------------

  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
  /*_____________________________________ 'Auto Lid Open' Setup____________________________________________*/
  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------

  //-----------------------------------------------------------------------
  /* Servo setup */
  servo.attach(Servo_Attach);
  servo.write(0);
  delay(1000);
  //-----------------------------------------------------------------------

  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
}

/************************************************************************************
 *  loop() function
 **********************************************************************************/
void loop()
{
  if (Serial.available() > 0)
  {
    value = Serial.parseInt();
    Serial.print("Weight of the bucket: ");
    Serial.println(value);
    Blynk.virtualWrite(V0, value);
  }
  Blynk.run();
  //  timer.run();
  Serial.println("I can do other things!!");

  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
  /*__________________________________ 'Auto Lid Open' run time execution  ________________________________*/
  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------

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

  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------------------------------
}
