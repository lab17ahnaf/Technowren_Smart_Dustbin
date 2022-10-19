int value = 10;

//-----------------------------------------------------------------------
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
//-----------------------------------------------------------------------
/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLlknAspjs"
#define BLYNK_DEVICE_NAME "Dustbin Project"
#define BLYNK_AUTH_TOKEN "ECb4CD9QWYuGiOV5ycC01kI9eoP_yhVj"
//-----------------------------------------------------------------------
// Select your modem:
#define TINY_GSM_MODEM_SIM800
//-----------------------------------------------------------------------
// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30
//-----------------------------------------------------------------------
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
//-----------------------------------------------------------------------
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;
//-----------------------------------------------------------------------
// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "blweb";
char user[] = "";
char pass[] = "";
//-----------------------------------------------------------------------
// Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1
//-----------------------------------------------------------------------
// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(7,10); // RX, TX
TinyGsm modem(SerialAT);

BlynkTimer timer;
#define INTERVAL 1000L

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


/************************************************************************************
 *  setup() function
 **********************************************************************************/
void setup()
{
  //-----------------------------------------------------------
  // Debug console
  Serial.begin(115200);
  delay(10);
  //-----------------------------------------------------------
  // Set GSM module baud rate
  SerialAT.begin(9600);
  //SerialAT.begin(9600,SWSERIAL_8N1,D3,D4);
  delay(3000);
  
  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
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
  if(Serial.available()>0){
    value = Serial.parseInt();
    Serial.print("Weight of the bucket: ");
    Serial.println(value);
    Blynk.virtualWrite(V0, value);
  }
  Blynk.run();
//  timer.run();
}
