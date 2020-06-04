/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/blynk.ino"
#include "blynk.h"

void setAppWidgets(Zstate *st);
void initializeAppWidgets(Zstate st);
#line 3 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/blynk.ino"
Zstate *blynkState = &currentState;  //a copy ... many blynk functions below cannot take state as a param.

// Update the App UI widgets as needed
void setAppWidgets(Zstate *st)
{
  Blynk.virtualWrite(V11, st->numberOfReadings);
  Blynk.virtualWrite(V13, st->secondsBetweenReadings);
  Blynk.virtualWrite(V15, st->zeroOff);
}

void initializeAppWidgets(Zstate st)
{
  Blynk.virtualWrite(V10, st->numberOfReadings);
  Blynk.virtualWrite(V12, st->secondsBetweenReadings);
  Blynk.virtualWrite(V11, st->numberOfReadings);
  Blynk.virtualWrite(V13, st->secondsBetweenReadings);
  Blynk.virtualWrite(V20, st->expression);
  Blynk.virtualWrite(V16, st->email);
  Blynk.virtualWrite(V18, st->batEmail);
  Blynk.virtualWrite(V26, st->tamperEmail);
  Blynk.virtualWrite(V15, st->zeroOff);
  Blynk.virtualWrite(V25, st->batThreshold);
  Blynk.virtualWrite(V23, st->deviceZone);
 // Blynk.virtualWrite(V24, st->buzzerActivated);
  Blynk.virtualWrite(V22,"Enter code to reset");
  // New Pins
  Blynk.virtualWrite(V24, st->buzzerTamper); // Buzzer Tamper
  Blynk.virtualWrite(V27, st->buzzerVapor); // Vapor Buzzer
  Blynk.virtualWrite(V28, st->notifyVapor); // Vapor Notify
  Blynk.virtualWrite(V29, st->notifyTamper); // Tamper Notify
  Blynk.virtualWrite(V32, st->notifyBattery); // Battery Notify
  if (st->ActivityThreshold==144)
  	Blynk.virtualWrite(V31, 3); // Tamper Setting Hammer
  else  if (st->ActivityThreshold==72)
  	Blynk.virtualWrite(V31, 1); // Tamper Setting feather
  else
  	Blynk.virtualWrite(V31, 2); // Tamper Setting default

  if (terminalDebug) Blynk.virtualWrite(V21, "Initialized default values");
  st->firstRunCheck = firstRunValue;
  st->bSleepModeStandby=true;
  st->bInSleepMode=false;
  debug("Finished Initialization\n");
  saveState();
}

// Update app connection ate
BLYNK_APP_CONNECTED() {
  appConnected = true;
  setAppWidgets(blynkState);
  debug("Connected\n");
}

BLYNK_APP_DISCONNECTED() {
// Your code here
 appConnected = false;
 debug("Disconnected\n");
}

// BLYNK_WRITE is a function called every time device gets an update of Virtual Pin value from the server (or app):
// Get the desired number of readings from the app
BLYNK_WRITE(V10)
{   
  blynkState->numberOfReadings = param.asInt(); // Get value as integer
  //Save
  saveState();
  setAppWidgets();
}

BLYNK_WRITE(V12)
{   
  blynkState->secondsBetweenReadings = param.asInt(); // Get value as integer
  //Save to FRAM
  saveState();
  setAppWidgets();
}

BLYNK_WRITE(V22) //Reset Expression
{
  String _resetKey = param.asStr();
  if (_resetKey.equals("resetme"))
  {
    Blynk.virtualWrite(V22,"Enter code to reset");
    zstate.clear(blynkState);
    
    blynkState->numberOfReadings = 15;
    blynkState->secondsBetweenReadings=5;
    blynkState->firstRunCheck = 0;
    blynkState->bSleepModeStandby=true;
    blynkState->bInSleepMode=false;
    blynkState->zeroOff = 0;
    strcpy(blynkState->expression,"Enter expression here");
    strcpy(blynkState->email,"Enter email here");
    strcpy(blynkState->batEmail,"Enter email here");
    strcpy(blynkState->tamperEmail,"Enter email here");
    blynkState->deviceZone = 0;
    blynkState->batThreshold = 20;
    blynkState->lastAlert = false;
    blynkState->batLastAlert = false;
    blynkState->ActivityThreshold = 100; // Which is set to 1 in Blynk
    blynkState->buzzerTamper = true;  // Buzzer Tamper
    blynkState->buzzerVapor = false; // Vapor Buzzer
    blynkState->notifyVapor = true; // Vapor Notify
    blynkState->notifyTamper = true; // Tamper Notify
    blynkState->notifyBattery = true; // Battery Notify
    blynkState->OnTime = 0;
    Blynk.virtualWrite(V10, blynkState->numberOfReadings);
    Blynk.virtualWrite(V12, blynkState->secondsBetweenReadings);
    Blynk.virtualWrite(V11, blynkState->numberOfReadings);
    Blynk.virtualWrite(V13, blynkState->secondsBetweenReadings);
    Blynk.virtualWrite(V20, blynkState->expression);
    Blynk.virtualWrite(V16, blynkState->email);
    Blynk.virtualWrite(V18, blynkState->batEmail);
    Blynk.virtualWrite(V26, blynkState->tamperEmail);
    Blynk.virtualWrite(V15, blynkState->zeroOff);
    Blynk.virtualWrite(V25, blynkState->batThreshold);
    Blynk.virtualWrite(V23, blynkState->deviceZone);
    Blynk.virtualWrite(V24, blynkState->buzzerTamper);  // Buzzer Tamper
    Blynk.virtualWrite(V27, blynkState->buzzerVapor); // Vapor Buzzer
    Blynk.virtualWrite(V28, blynkState->notifyVapor); // Vapor Notify
    Blynk.virtualWrite(V29, blynkState->notifyTamper); // Tamper Notify
    Blynk.virtualWrite(V31, 2); // Which is 100 for the accelerometer
    Blynk.virtualWrite(V32, blynkState->notifyBattery); // Battery Notify
    // EEPROM.clear();
    fram.erase();
    Blynk.virtualWrite(V21, "Hard Resetting");
    debug("Hard Resetting\n");
    delay(500);
    System.reset();
  }
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  debug("Blynk is now connected - syncing all pins.\n");
  if (blynkState->firstRunCheck != firstRunValue)
  {
    initializeAppWidgets();
  }
  Blynk.syncAll();
  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);
}

BLYNK_WRITE(V20) //Alarm Expression
{
  String _expression = param.asStr();
  //Only lower case is allowed
  _expression.toLowerCase();
  strcpy(blynkState->expression,_expression.c_str());
  saveState();
  if (sensorValid)
  {
    runExpression(blynkState->expression);
  }
}

BLYNK_WRITE(V16) //Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
  {
    strcpy(blynkState->email,_email.c_str());
    saveState();
  }
  else
  {
    Blynk.virtualWrite(V16, "Invalid address");
  }
  
}

BLYNK_WRITE(V18) //Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
  {
    strcpy(blynkState->batEmail,_email.c_str());
    saveState();
  }
  else
  {
    Blynk.virtualWrite(V18, "Invalid address");
  }
  
}

BLYNK_WRITE(V26) //Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
  {
    strcpy(blynkState->tamperEmail,_email.c_str());
    saveState();
  }
  else
  {
    Blynk.virtualWrite(V26, "Invalid address");
  }
  
}
// Calibration
BLYNK_WRITE(V14) 
{
  if (param.asInt()==1)
  {
    blynkState->zeroOff = sensor1.zero();
    saveState();
    Blynk.virtualWrite(V15, blynkState->zeroOff);
  }
  Blynk.virtualWrite(V14, 0);
  // Since data will be invalid save the new calibration and reset
  delay(4000);
  System.reset();
}

//Battery threshold
BLYNK_WRITE(V25) 
{
  float thresh = param.asFloat();
  blynkState->batThreshold=thresh;
  saveState();
}

BLYNK_WRITE(V17) 
{
  #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
  #endif
  if (param.asInt()==1)
  {
    terminalDebug = false;
  }
  else
  {
    terminalDebug = true;
  }
}

// Update time zone
BLYNK_WRITE(V23) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    blynkState->deviceZone=param.asInt();
#ifdef KURTDEBUG
    debug("*****Device Zone value is: ");
    debug(blynkState->deviceZone + "\n");
#endif
    gmtOffsetValid=false;
  saveState();
}
// Update buzzerTamper flag
BLYNK_WRITE(V24) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    blynkState->buzzerTamper=param.asInt();
  saveState();

#ifdef KURTDEBUG
    debug("*****Update buzzerTamper flag value is: ");
    debug(blynkState->buzzerTamper + "\n");
#endif
}
// Update buzzerVapor flag
BLYNK_WRITE(V27) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    blynkState->buzzerVapor=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("*****Update buzzerVapor flag value is: ");
    debug(blynkState->buzzerVapor + "\n");
#endif
}
// Update notifyVapor flag
BLYNK_WRITE(V28) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    blynkState->notifyVapor=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("*****Update notifyVapor flag value is: ");
    debug(blynkState->notifyVapor + "\n");
#endif
}
// Update notifyTamper flag
BLYNK_WRITE(V29) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    blynkState->notifyTamper=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("*****Update notifyTamper flag value is: ");
    debug(blynkState->notifyTamper + "\n");
#endif
}

//Accelerometer threshold
BLYNK_WRITE(V31) 
{
  int thresh = param.asInt();
  int oldvalue=blynkState->ActivityThreshold;

#ifdef KURTDEBUG
    debug("*****Update Accelerometer threshold value is: ");
    debug(thresh + "\n");
#endif
  blynkState->ActivityThreshold=100;
  if (thresh==3)
  	blynkState->ActivityThreshold=144;
  if (thresh==2)
  	blynkState->ActivityThreshold=100;
  if (thresh==1)
  	blynkState->ActivityThreshold=72;
  // If value changed since the acceleromter will need to be reinitalized save the new value and reset
  if (blynkState->ActivityThreshold != oldvalue)
  {
#ifdef KURTDEBUG
    debug("Update Accelerometer value is: ");
    debug(blynkState->ActivityThreshold + "\n");
    debug("Will reset system ");
#endif
    saveState();
    delay(4000);
    System.reset();
  }
}

// Update notifyBattery flag
BLYNK_WRITE(V32) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    blynkState->notifyBattery=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("Update notifyBattery flag value is: ");
    debug(blynkState->notifyBattery + "\n");
#endif
}
