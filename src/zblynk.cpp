// emacs -*- c++ -*-
//zblynk - glue code between blynk and zeptive
#include "application.h"

#include "blynk.h"
#include "zblynk.h"
#include "zdetector.h"

#define BLYNKGREEN 0
#define BLYNKRED 1

Zblynk zblynk;

void Zblynk::setup() {  zblynk_connected = false;   Blynk.run();  }

bool Zblynk::isConnected() { return zblynk_connected;  }

void Zblynk::write_numberOfReadings(int numOfReadings)
{
  Blynk.virtualWrite(V10, numOfReadings);
  Blynk.virtualWrite(V11, numOfReadings);
}

void Zblynk::write_secondsBetweenReadings(int secBetwReadings)
{
  Blynk.virtualWrite(V12, secBetwReadings);
  Blynk.virtualWrite(V13, secBetwReadings);
}

void Zblynk::redAlert()		{  Blynk.virtualWrite(V9, BLYNKRED);  }
void Zblynk::greenAlert()	{  Blynk.virtualWrite(V9, BLYNKGREEN);  }

void Zblynk::status_message(String msg)		{  Blynk.virtualWrite(V30, msg);  }
void Zblynk::debug_message(String msg)  	{  Blynk.virtualWrite(V21, msg);  }
void Zblynk::write_zeroOffset(float zo)		{  Blynk.virtualWrite(V15, zo);  }
void Zblynk::write_expression(char *expr)	{  Blynk.virtualWrite(V20, expr);  }
void Zblynk::write_batteryThreshold(float thr)	{  Blynk.virtualWrite(V25, thr);  }
void Zblynk::write_deviceTimeZone(int tz)	{  Blynk.virtualWrite(V23, tz);  }
void Zblynk::write_enableVapeBuzzer(bool vb)	{  Blynk.virtualWrite(V27, vb);  }
void Zblynk::write_enableTamperBuzzer(bool tb)	{  Blynk.virtualWrite(V24, tb);  }
void Zblynk::write_enableVapeNotify(bool vn)	{  Blynk.virtualWrite(V28, vn);  }
void Zblynk::write_enableBatteryNotify(bool bn)	{  Blynk.virtualWrite(V32, bn);  }
void Zblynk::write_enableTamperNotify(bool tn)	{  Blynk.virtualWrite(V29, tn);  }
void Zblynk::write_vapeEmail(char *email)	{  Blynk.virtualWrite(V16, email);  }
void Zblynk::write_batteryEmail(char *email)	{  Blynk.virtualWrite(V18, email);  }
void Zblynk::write_tamperEmail(char *email)	{  Blynk.virtualWrite(V26, email);  }
void Zblynk::write_enterCode(char *msg)	{  Blynk.virtualWrite(V22, msg);  }

void Zblynk::write_activityThreshold(int at)
{
  int outp = 2;
  if (at == 144) outp = 3;
  else if (at == 72) outp = 1;
  Blynk.virtualWrite(V31, outp);
}

void Zblynk::update_all_state()
{
  write_numberOfReadings(zstate.p.numberOfReadings);
  write_secondsBetweenReadings(zstate.p.secondsBetweenReadings);
  write_zeroOffset(zstate.p.zeroOffset);
  write_expression(zstate.p.expression);
  write_batteryThreshold(zstate.p.batteryThreshold);
  write_deviceTimeZone(zstate.p.deviceTimeZone);
  
  write_enableVapeBuzzer(zstate.p.enableBuzzerVape);
  write_enableTamperBuzzer(zstate.p.enableBuzzerTamper);
  //write_enableBatteryBuzzer(zstate.p.enableBuzzerBattery);
  
  write_enableVapeNotify(zstate.p.enableNotifyVape);
  write_enableBatteryNotify(zstate.p.enableNotifyBattery);
  write_enableTamperNotify(zstate.p.enableNotifyTamper);
  write_vapeEmail(zstate.p.vapeEmail);
  write_batteryEmail(zstate.p.batteryEmail);
  write_tamperEmail(zstate.p.tamperEmail);
  write_activityThreshold(zstate.p.activityThreshold);
  write_enterCode("Enter code to reset");
}

void Zblynk::write_pm1()		{  String str = String::format("%0.1f", zdetector.pm1());		Blynk.virtualWrite(V1, str); }
void Zblynk::write_pm25()		{  String str = String::format("%0.1f", zdetector.pm25());		Blynk.virtualWrite(V2, str); }
void Zblynk::write_pm4()		{  String str = String::format("%0.1f", zdetector.pm4());		Blynk.virtualWrite(V3, str); }
void Zblynk::write_pm10()		{  String str = String::format("%0.1f", zdetector.pm10());		Blynk.virtualWrite(V4, str); }
void Zblynk::write_gas_concentration()	{  String str = String::format("%0.1f", zdetector.gas_concentration()); Blynk.virtualWrite(V5, str); }
void Zblynk::write_tempF()		{  String str = String::format("%0.1f", zdetector.tempF());		Blynk.virtualWrite(V6, str); }
void Zblynk::write_batteryLevel()	{  String str = String::format("%0.1f", zdetector.batteryLevel());	Blynk.virtualWrite(V7, str); }
void Zblynk::write_signalStrength()	{  String str = String::format("%0.1f", zdetector.signalStrength());	Blynk.virtualWrite(V8, str); }

void Zblynk::update_all_readings()
{
  write_pm1();
  write_pm25();
  write_pm4();
  write_pm10();
  write_batteryLevel();
  write_gas_concentration();
  write_tempF();
  write_signalStrength();
}

// Write the App UI widgets as needed
void Zblynk::update()
{
  write_numberOfReadings();
  write_secondsBetweenReadings();
  write_zeroOffset();
  //Blynk.virtualWrite(V11, zstate.p.numberOfReadings);
  //Blynk.virtualWrite(V13, zstate.p.secondsBetweenReadings);
  //Blynk.virtualWrite(V15, zstate.p.zeroOff);
}

// Update app connection state
BLYNK_APP_CONNECTED() {
  zblynk_connected = true;
  update_all();
  debug("Connected\n");
}

BLYNK_APP_DISCONNECTED() {
  // Your code here
  zblynk_connected = false;
  debug("Disconnected\n");
}

// BLYNK_WRITE is a function called every time device gets an update of Virtual Pin value from the server (or app):
// Get the desired number of readings from the app
BLYNK_WRITE(V10)
{   
  zstate.p.numberOfReadings = param.asInt(); // Get value as integer
  zstate.save();
  zblynk.update();
}

BLYNK_WRITE(V12)
{   
  zstate.p.secondsBetweenReadings = param.asInt(); // Get value as integer
  zstate.save();
  zblynk.update();
}

BLYNK_WRITE(V22) //Reset Expression
{
  String _resetKey = param.asStr();
  if (_resetKey.equals("resetme")) {
    write_enterCode("Enter code to reset");
    zstate.p.factory_reset();
    update_all();
    fram.erase();
    zblynk.debug_message("Hard Resetting");
    delay(500);
    System.reset();
  }
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  debug("Blynk is now connected - syncing all pins.\n");
  if (zstate.p.firstRunCheck != firstRunValue)
      update_all();
  Blynk.syncAll();
  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);
}

BLYNK_WRITE(V20) //Alarm Expression
{
  String _expression = param.asStr();
  //Only lower case is allowed
  _expression.toLowerCase();
  strcpy(zstate.p.expression,_expression.c_str());
  zstate.save();
}

BLYNK_WRITE(V16) //Vape Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail) {
      strcpy(zstate.p.email, _email.c_str());
      zstate.save();
    }
  else Blynk.virtualWrite(V16, "Invalid address");
}

BLYNK_WRITE(V18) //Battery Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail) {
      strcpy(zstate.p.batEmail, _email.c_str());
      zstate.save();
    }
  else Blynk.virtualWrite(V18, "Invalid address");
}

BLYNK_WRITE(V26) //Tamper Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail) {
      strcpy(zstate.p.tamperEmail, _email.c_str());
      zstate.save();
    }
  else Blynk.virtualWrite(V26, "Invalid address");
}

// Calibration
BLYNK_WRITE(V14) 
{
  if (param.asInt() == 1) {
      zstate.p.zeroOffset = ziaq.zero();
      zstate.save();
      write_zeroOffset(zstate.p.zeroOffset);
  }
  Blynk.virtualWrite(V14, 0);
  // Since data will be invalid save the new calibration and reset
  delay(4000);
  System.reset();
}

//Battery threshold
BLYNK_WRITE(V25) {  zstate.p.batteryThreshold = param.asFloat();  zstate.save();  }

BLYNK_WRITE(V17) {  zstate.p.terminalDebug = (param.asInt() == 1);  zstate.save(); }

// Update buzzerTamper flag
BLYNK_WRITE(V24) {  zstate.p.enableBuzzerTamper = param.asInt();  zstate.save();  }

// Update buzzerVapor flag
BLYNK_WRITE(V27) {  zstate.p.enableBuzzerVapor = param.asInt();  zstate.save();  }

// Update notifyVapor flag
BLYNK_WRITE(V28) {  zstate.p.enableNotifyVapor = param.asInt();  zstate.save();  }

// Update notifyTamper flag
BLYNK_WRITE(V29) {  zstate.p.enableNotifyTamper = param.asInt();  zstate.save();  }

// Update notifyBattery flag
BLYNK_WRITE(V32) {  zstate.p.enableNotifyBattery = param.asInt();  zstate.save();  }

// Update time zone
BLYNK_WRITE(V23) 
{
  zstate.p.deviceTimeZone = param.asInt();
  zstate.p.gmtOffsetValid = false;
  zclock.now();	//get an update, triggering cloud sync
  zstate.save();
}

//Accelerometer threshold
BLYNK_WRITE(V31) 
{
  int thresh = param.asInt();
  int oldvalue = zstate.p.ActivityThreshold;

  switch (thresh) {
  case 1:	zstate.p.ActivityThreshold =  72;	break;
  case 3:	zstate.p.ActivityThreshold = 144;	break;
  default:	zstate.p.ActivityThreshold = 100;	break;
  }
  // If value changed since the acceleromter will need to be reinitalized save the new value and reset
  if (zstate.p.ActivityThreshold != oldvalue) {
    zstate.save();
    delay(4000);
    System.reset();
  }
}

