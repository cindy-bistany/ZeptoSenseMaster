/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/protection.ino"
#include "SparkFun_ADXL345ST.h"         // SparkFun ADXL345 Library

//********** ADXL345 COMMUNICATION ***********
void ADXL_ISR();
void accel_poweron();
void accel_check();
void setup_adxl345();
#line 4 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/protection.ino"
ADXL345 adxl = ADXL345();             // USE FOR ACCEL COMMUNICATION in I2C mode

//******************** ISR *********************
// Look for Interrupts and Triggered Action    
// Need to keep this short and must not include I2C etc.
void ADXL_ISR() {
  accelInterrupt=true; 
}

void accel_poweron() { adxl.powerOn(); }

void accel_check()
{
  // Not ideal to have this in the loop
  if (accelInterrupt)
  {
    debug("*** ACCEL INTERRUPTED ***\n");
    // Turn off Interrupts for Activity(1 == ON, 0 == OFF)
    adxl.InactivityINT(0);
    adxl.ActivityINT(0);
    // getInterruptSource clears all triggered actions after returning value
    // Do not call again until you need to recheck for triggered actions
    byte interrupts = adxl.getInterruptSource();

    // Inactivity
    if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
      debug("*** INACTIVITY ***\n");
      // *Temp Debug* Turn off the LED.
      // digitalWrite(ledPin, LOW);
      //Robert add code here for Blynk to do when inactivity is sensed
      tamperCurrentAlert = false;
      if (tamperAlertChanged())
      {
        /*#ifdef Version_2
        Blynk.logEvent("tamper_alert_ended");
        #else
        Blynk.email(state.tamperEmail, "{DEVICE_NAME} : Tamper alarm ended", "{DEVICE_NAME} tamper alarm ended.");
        #endif  */   
      }
      // Toggle interrupts - i.e after inactivity check for activity
      adxl.ActivityINT(1);
    }
    // Activity
    if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
      debug("*** ACTIVITY ***\n");
      // *Temp Debug* Turn on the LED.
      // digitalWrite(ledPin, HIGH);
      //Robert add code here for Blynk to do when activity is sensed
      tamperCurrentAlert = true;
      if (tamperAlertChanged())
      {
        #ifdef Version_2
        if (state.notifyTamper==true)
          Blynk.logEvent("tamper_alert");
        #else
        Blynk.email(state.tamperEmail, "{DEVICE_NAME} : Tamper alarm", "{DEVICE_NAME} tamper alarm!");
        #endif      
      }
      // Toggle interrupts - i.e after activity check for inactivity
      adxl.InactivityINT(1);
    }
    accelInterrupt=false;
  }

// Only buzzer for tamper 
  if ((tamperCurrentAlert==false)&&(!VapeBuzzerOn))
  {
    //Buzzer Off
    digitalWrite(buzzer, LOW);
  }

  if ((tamperCurrentAlert==true)&&(state.buzzerTamper==true))
  {
    //Buzzer On
    if (state.buzzerTamper)
      digitalWrite(buzzer, HIGH);
  }

}

void setup_adxl345()
{
  /////////////
  // Setup ADXL345
  debug("ADXL345 Setup");
  debug("\n");
  // // *Temp Debug* Set the pin mode to output, so you may control it.
  // pinMode(ledPin, OUTPUT);
  // Power on the ADXL345
  adxl.powerOn();

  // Configure Chip
  adxl.setRangeSetting(4);            // Give the range settings
  // Accepted values are 2g, 4g, 8g or 16g
  // Higher Values = Wider Measurement Range
  // Lower Values = Greater Sensitivity

  // No TAP or Freefall detection only Activity
  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(state.ActivityThreshold);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255) 36,72,100 or 144

  adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(state.ActivityThreshold);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
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

  delay(300);  
}

setup_accelerometer()
{
  setup_adxl345();
}

setup_protection()
{
  setup_accelerometer();
}
