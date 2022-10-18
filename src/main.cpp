#include <Arduino.h>
#include <_config.h>

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
}
