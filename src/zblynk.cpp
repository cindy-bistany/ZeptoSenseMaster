// emacs -*- c++ -*-
//zblynk - glue code between blynk and zeptive

#include "blynk.h"
#include "zblynk.h"

Zblynk zblynk;

void Zblynk::init(Zstate *st)
{
  zblynk.st = st; //save pointer to state
}

void Zblynk::status_message(string msg)
{
  Blynk.virtualWrite(V30, msg);
}

void Zblynk::debug_message(string msg)
{
  Blynk.virtualWrite(V21, msg);
}

// Update the App UI widgets as needed
void Zblynk::setAppWidgets(Zstate *st)
{
  Blynk.virtualWrite(V11, st->numberOfReadings);
  Blynk.virtualWrite(V13, st->secondsBetweenReadings);
  Blynk.virtualWrite(V15, st->zeroOff);
}

void Zblynk::initializeAppWidgets(Zstate *st)
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

  if (terminalDebug) zblynk.debug_message("Initialized default values");
  st->firstRunCheck = firstRunValue;
  st->bSleepModeStandby=true;
  st->bInSleepMode=false;
  debug("Finished Initialization\n");
  zblynk.st->save();
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
  zblynk.st->numberOfReadings = param.asInt(); // Get value as integer
  //Save
  zblynk.st->save();
  zblynk.setAppWidgets(zblynk.st);
}

BLYNK_WRITE(V12)
{   
  zblynk.st->secondsBetweenReadings = param.asInt(); // Get value as integer
  //Save to FRAM
  zblynk.st->save();
  zblynk.setAppWidgets(zblynk.st);
}

BLYNK_WRITE(V22) //Reset Expression
{
  string _resetKey = param.asStr();
  if (_resetKey.equals("resetme"))
    {
      Blynk.virtualWrite(V22,"Enter code to reset");
      zstate.clear(blynkState);
    
      zblynk.st->numberOfReadings = 15;
      zblynk.st->secondsBetweenReadings=5;
      zblynk.st->firstRunCheck = 0;
      zblynk.st->bSleepModeStandby=true;
      zblynk.st->bInSleepMode=false;
      zblynk.st->zeroOff = 0;
      strcpy(zblynk.st->expression,"Enter expression here");
      strcpy(zblynk.st->email,"Enter email here");
      strcpy(zblynk.st->batEmail,"Enter email here");
      strcpy(zblynk.st->tamperEmail,"Enter email here");
      zblynk.st->deviceZone = 0;
      zblynk.st->batThreshold = 20;
      zblynk.st->lastAlert = false;
      zblynk.st->batLastAlert = false;
      zblynk.st->ActivityThreshold = 100; // Which is set to 1 in Blynk
      zblynk.st->buzzerTamper = true;  // Buzzer Tamper
      zblynk.st->buzzerVapor = false; // Vapor Buzzer
      zblynk.st->notifyVapor = true; // Vapor Notify
      zblynk.st->notifyTamper = true; // Tamper Notify
      zblynk.st->notifyBattery = true; // Battery Notify
      zblynk.st->OnTime = 0;
      Blynk.virtualWrite(V10, zblynk.st->numberOfReadings);
      Blynk.virtualWrite(V12, zblynk.st->secondsBetweenReadings);
      Blynk.virtualWrite(V11, zblynk.st->numberOfReadings);
      Blynk.virtualWrite(V13, zblynk.st->secondsBetweenReadings);
      Blynk.virtualWrite(V20, zblynk.st->expression);
      Blynk.virtualWrite(V16, zblynk.st->email);
      Blynk.virtualWrite(V18, zblynk.st->batEmail);
      Blynk.virtualWrite(V26, zblynk.st->tamperEmail);
      Blynk.virtualWrite(V15, zblynk.st->zeroOff);
      Blynk.virtualWrite(V25, zblynk.st->batThreshold);
      Blynk.virtualWrite(V23, zblynk.st->deviceZone);
      Blynk.virtualWrite(V24, zblynk.st->buzzerTamper);  // Buzzer Tamper
      Blynk.virtualWrite(V27, zblynk.st->buzzerVapor); // Vapor Buzzer
      Blynk.virtualWrite(V28, zblynk.st->notifyVapor); // Vapor Notify
      Blynk.virtualWrite(V29, zblynk.st->notifyTamper); // Tamper Notify
      Blynk.virtualWrite(V31, 2); // Which is 100 for the accelerometer
      Blynk.virtualWrite(V32, zblynk.st->notifyBattery); // Battery Notify
      // EEPROM.clear();
      fram.erase();
      zblynk.debug_message("Hard Resetting");
      debug("Hard Resetting\n");
      delay(500);
      System.reset();
    }
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  debug("Blynk is now connected - syncing all pins.\n");
  if (zblynk.st->firstRunCheck != firstRunValue)
    {
      initializeAppWidgets();
    }
  Blynk.syncAll();
  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);
}

BLYNK_WRITE(V20) //Alarm Expression
{
  string _expression = param.asStr();
  //Only lower case is allowed
  _expression.toLowerCase();
  strcpy(zblynk.st->expression,_expression.c_str());
  zblynk.st->save();
  if (sensorValid)
    {
      runExpression(zblynk.st->expression);
    }
}

BLYNK_WRITE(V16) //Email Expression
{
  string _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
    {
      strcpy(zblynk.st->email,_email.c_str());
      zblynk.st->save();
    }
  else
    {
      Blynk.virtualWrite(V16, "Invalid address");
    }
  
}

BLYNK_WRITE(V18) //Email Expression
{
  string _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
    {
      strcpy(zblynk.st->batEmail,_email.c_str());
      zblynk.st->save();
    }
  else
    {
      Blynk.virtualWrite(V18, "Invalid address");
    }
  
}

BLYNK_WRITE(V26) //Email Expression
{
  string _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
    {
      strcpy(zblynk.st->tamperEmail,_email.c_str());
      zblynk.st->save();
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
      zblynk.st->zeroOff = sensor1.zero();
      zblynk.st->save();
      Blynk.virtualWrite(V15, zblynk.st->zeroOff);
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
  zblynk.st->batThreshold=thresh;
  zblynk.st->save();
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
  zblynk.st->deviceZone=param.asInt();
#ifdef KURTDEBUG
  debug("*****Device Zone value is: ");
  debug(zblynk.st->deviceZone + "\n");
#endif
  gmtOffsetValid=false;
  zblynk.st->save();
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
  zblynk.st->buzzerTamper=param.asInt();
  zblynk.st->save();

#ifdef KURTDEBUG
  debug("*****Update buzzerTamper flag value is: ");
  debug(zblynk.st->buzzerTamper + "\n");
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
  zblynk.st->buzzerVapor=param.asInt();
  zblynk.st->save();
#ifdef KURTDEBUG
  debug("*****Update buzzerVapor flag value is: ");
  debug(zblynk.st->buzzerVapor + "\n");
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
  zblynk.st->notifyVapor=param.asInt();
  zblynk.st->save();
#ifdef KURTDEBUG
  debug("*****Update notifyVapor flag value is: ");
  debug(zblynk.st->notifyVapor + "\n");
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
  zblynk.st->notifyTamper=param.asInt();
  zblynk.st->save();
#ifdef KURTDEBUG
  debug("*****Update notifyTamper flag value is: ");
  debug(zblynk.st->notifyTamper + "\n");
#endif
}

//Accelerometer threshold
BLYNK_WRITE(V31) 
{
  int thresh = param.asInt();
  int oldvalue = zblynk.st->ActivityThreshold;

#ifdef KURTDEBUG
  debug("*****Update Accelerometer threshold value is: ");
  debug(thresh + "\n");
#endif
  zblynk.st->ActivityThreshold=100;
  if (thresh==3)
    zblynk.st->ActivityThreshold=144;
  if (thresh==2)
    zblynk.st->ActivityThreshold=100;
  if (thresh==1)
    zblynk.st->ActivityThreshold=72;
  // If value changed since the acceleromter will need to be reinitalized save the new value and reset
  if (zblynk.st->ActivityThreshold != oldvalue) {
#ifdef KURTDEBUG
      debug("Update Accelerometer value is: ");
      debug(zblynk.st->ActivityThreshold + "\n");
      debug("Will reset system ");
#endif
      zblynk.st->save();
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
  zblynk.st->notifyBattery=param.asInt();
  zblynk.st->save();
#ifdef KURTDEBUG
  debug("Update notifyBattery flag value is: ");
  debug(zblynk.st->notifyBattery + "\n");
#endif
}
