#include "SparkFun_ADXL345ST.h"         // SparkFun ADXL345 Library
#include "zutil.h"
#include "ztamper.h"
#include "zstate.h"
#include "zblynk.h"

//********** ADXL345 COMMUNICATION ***********

//******************** ISR *********************
// Look for Interrupts and Triggered Action    
// Need to keep this short and must not include I2C etc.
#define ADXL_interruptPin = A0;
ADXL345 adxl = ADXL345();             // USE FOR ACCEL COMMUNICATION in I2C mode
static bool adxlInterruptTriggered = false;
void ADXL_ISR() { adxlInterruptTriggered = true; }	//this gets connected in setup_adxl()
//**********************************************

void Ztamper.setup() {
  setup_adxl345();
  reset();
}

bool Ztamper::tampered() { return isRising() || isHigh() || isFalling(); }
void Ztamper::reset() { tamperInterruptTriggered = tamperInterruptTriggered_prev = false; }

void Ztamper::rise()
{
#ifdef Version_2
	if (zstate.p.notifyTamper == true) Blynk.logEvent("tamper_alert");
#else
	Blynk.email(zstate.p.tamperEmail, "{DEVICE_NAME} : Tamper alarm", "{DEVICE_NAME} tamper alarm!");
#endif      
  if (zstate.p.buzzerTamper) buzzon();
}

void Ztamper::stayHigh()
{
}

void Ztamper::fall()
{
  //  buzzoff;
}

void Ztamper::stayLow()
{
  //  buzzoff;
}

bool Ztamper::isRising()	{ return  tamperInterruptTriggered && !tamperInterruptTriggered_prev; }
bool Ztamper::isHigh()		{ return  tamperInterruptTriggered &&  tamperInterruptTriggered_prev; }
bool Ztamper::isFalling()	{ return !tamperInterruptTriggered &&  tamperInterruptTriggered_prev; }
bool Ztamper::isLow()		{ return !tamperInterruptTriggered && !tamperInterruptTriggered_prev; }

void Ztamper::loop_process_adxlinterrupt()
{
  tamperInterruptTriggered_prev = tamperInterruptTriggered;
  tamperInterruptTriggered = false;	//let's see if any possible adxl int is really a tamper int
  
  if (adxlInterruptTriggered) {
    adxl.InactivityINT(0);	// Turn off Interrupts for Activity(1 == ON, 0 == OFF)
    adxl.ActivityINT(0);
    // getInterruptSource clears all triggered actions after returning value
    // Do not call again until you need to recheck for triggered actions
    byte interrupts = adxl.getInterruptSource();	//must be a bit mask I guess

    // Inactivity
    if (adxl.triggered(interrupts, ADXL345_INACTIVITY)) {
      //seems like a good place here for a heartbeat message back to HQ
      //
    }
    
    // Activity
    tamperInterruptTriggered = adxl.triggered(interrupts, ADXL345_ACTIVITY);
    
    adxl.InactivityINT(1);	// Turn off Interrupts for Activity(1 == ON, 0 == OFF)
    adxl.ActivityINT(1);
    adxlInterruptTriggered = false;
  }
}

void Ztamper::loop()
{
  loop_process_adxl_interrupt();
  
  if (isRising()) rise();
  else if (isHigh()) stayHigh();
  else if (isFalling()) fall();
  else if (isLow()) stayLow();
  else crash("Ztamper::loop() - Logic error\n");
}

/////////////
// Setup ADXL345
void Ztamper::setup_adxl345()
{
  debug("ADXL345 Setup\n");
  adxl.powerOn();

  // Configure Chip
  adxl.setRangeSetting(4);            // Give the range settings
  // Accepted values are 2g, 4g, 8g or 16g
  // Higher Values = Wider Measurement Range
  // Lower Values = Greater Sensitivity

  // No TAP or Freefall detection only Activity
  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(zstate.p.activityThreshold);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255) 36,72,100 or 144

  adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(zstate.p.activityThreshold);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  // Setting all interupts to take place on INT1 pin
  adxl.setImportantInterruptMapping(0, 0, 0, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
  // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.

  // Turn on Interrupts for Activity(1 == ON, 0 == OFF)
  // These are both set on to start and then toggled
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);

  // Need to set int1 pin as an input before calling atttachInterrupt
  pinMode(interruptPin,INPUT);
  attachInterrupt(interruptPin, ADXL_ISR, CHANGE);
}

