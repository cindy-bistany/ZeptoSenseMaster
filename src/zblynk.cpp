// emacs -*- c++ -*-
//zblynk - glue code between blynk and zeptive
#include "application.h"

#ifndef BLYNK_H
#define BLYNKGREEN 0
#define BLYNKRED 1
#define BLYNK_PRINT Serial1 // Defines the object that is used for printing
#define BLYNK_DEBUG        // Optional, this enables more detailed prints
#define BLYNK_HEARTBEAT      60
#include <blynk.h>
#define BLYNK_H
#endif

#define ZBLYNKPORT 8080

//
#include "zblynk.h"
#include "zdetector.h"
#include "zbackhaul.h"
#include "zbaseboard.h"
#include "ziaq.h"
#include "zclock.h"

String StateString = "RDY";

BlynkTimer sensorTimer; // Create a Timer object called "sensorTimer"!
BlynkTimer accelTimer; // Create an accelerometer Timer object

Zblynk zblynk;

void blynk_timer_callback() { zblynk.updateAll(); }

void Zblynk::setup()
{
  Blynk.config(auth, "zeptosense2.blynk.cc", ZBLYNKPORT);
  sensorTimer.setInterval((long) zstate.p.secondsBetweenReadings * 1000, blynk_timer_callback);
  sensorTimer.run();
  accelTimer.setInterval(111, blynk_timer_callback);
  accelTimer.run();

  zbaseboard.horn(32);
  Blynk.run();
  zbaseboard.horn(31);

  if (Blynk.connect()) zbaseboard.horn(41);
  else zbaseboard.horn(42);
  
  zbaseboard.horn(33);
  Blynk.connect();
  zbaseboard.horn(35);
  Blynk.run();
}

bool Zblynk::isConnected() { return Blynk.connected();  }

void Zblynk::logEvent(char *s) { Blynk.logEvent(s); }
  
void Zblynk::status_message(String msg)		{  Blynk.virtualWrite(V30, msg);  }
void Zblynk::debug_message(String msg)  	{  Blynk.virtualWrite(V21, msg);  }

void Zblynk::redAlert()		{  Blynk.virtualWrite(V9, BLYNKRED);  }
void Zblynk::greenAlert()	{  Blynk.virtualWrite(V9, BLYNKGREEN);  }

void Zblynk::write_numberOfReadings()
{
  Blynk.virtualWrite(V10, zstate.p.numberOfReadings);
  Blynk.virtualWrite(V11, zstate.p.numberOfReadings);
}

void Zblynk::write_secondsBetweenReadings()
{
  Blynk.virtualWrite(V12, zstate.p.secondsBetweenReadings);
  Blynk.virtualWrite(V13, zstate.p.secondsBetweenReadings);
}

void Zblynk::write_zeroOffset()		{  Blynk.virtualWrite(V15, zstate.p.zeroOffset);  }
void Zblynk::write_expression()		{  Blynk.virtualWrite(V20, zstate.p.expression);  }
void Zblynk::write_batteryThreshold()	{  Blynk.virtualWrite(V25, zstate.p.batteryThreshold);  }
void Zblynk::write_deviceTimeZone()	{  Blynk.virtualWrite(V23, zstate.p.deviceTimeZone);  }
void Zblynk::write_enableVapeBuzzer()	{  Blynk.virtualWrite(V27, zstate.p.enableBuzzerVape);  }
void Zblynk::write_enableTamperBuzzer()	{  Blynk.virtualWrite(V24, zstate.p.enableBuzzerTamper);  }
void Zblynk::write_enableVapeNotify()	{  Blynk.virtualWrite(V28, zstate.p.enableNotifyVape);  }
void Zblynk::write_enableBatteryNotify(){  Blynk.virtualWrite(V32, zstate.p.enableNotifyBattery);  }
void Zblynk::write_enableTamperNotify()	{  Blynk.virtualWrite(V29, zstate.p.enableNotifyTamper);  }
void Zblynk::write_vapeEmail()		{  Blynk.virtualWrite(V16, zstate.p.vapeEmail);  }
void Zblynk::write_batteryEmail()	{  Blynk.virtualWrite(V18, zstate.p.batteryEmail);  }
void Zblynk::write_tamperEmail()	{  Blynk.virtualWrite(V26, zstate.p.tamperEmail);  }
void Zblynk::write_enterCode()		{  Blynk.virtualWrite(V22, "Enter code to reset");  }

void Zblynk::write_activityThreshold()
{
  int outp = 2;
  if (zstate.p.activityThreshold == 144) outp = 3;
  else if (zstate.p.activityThreshold == 72) outp = 1;
  Blynk.virtualWrite(V31, outp);
}

void Zblynk::updateState()
{
  write_numberOfReadings();
  write_secondsBetweenReadings();
  write_zeroOffset();
  write_expression();
  write_batteryThreshold();
  write_deviceTimeZone();
  
  write_enableVapeBuzzer();
  write_enableTamperBuzzer();
  //write_enableBatteryBuzzer();
  
  write_enableVapeNotify();
  write_enableBatteryNotify();
  write_enableTamperNotify();
  write_vapeEmail();
  write_batteryEmail();
  write_tamperEmail();
  write_activityThreshold();
  write_enterCode();
}

void Zblynk::write_pm1()		{  String str = String::format("%0.1f", zdetector.pm1());		Blynk.virtualWrite(V1, str); }
void Zblynk::write_pm25()		{  String str = String::format("%0.1f", zdetector.pm25());		Blynk.virtualWrite(V2, str); }
void Zblynk::write_pm4()		{  String str = String::format("%0.1f", zdetector.pm4());		Blynk.virtualWrite(V3, str); }
void Zblynk::write_pm10()		{  String str = String::format("%0.1f", zdetector.pm10());		Blynk.virtualWrite(V4, str); }
void Zblynk::write_gas_concentration()	{  String str = String::format("%0.1f", zdetector.gas_concentration()); Blynk.virtualWrite(V5, str); }
void Zblynk::write_tempF()		{  String str = String::format("%0.1f", zdetector.tempF());		Blynk.virtualWrite(V6, str); }
void Zblynk::write_batteryLevel()	{  String str = String::format("%0.1f", zdetector.batteryLevel());	Blynk.virtualWrite(V7, str); }
void Zblynk::write_signalStrength()	{  String str = String::format("%0.1f", zbackhaul.signalStrength());	Blynk.virtualWrite(V8, str); }

void Zblynk::updateReadings()
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

void Zblynk::updateAll() {
  updateState();
  updateReadings();
}

// Update app connection state
BLYNK_APP_CONNECTED() {
  zblynk.updateAll();
  debug("Connected\n");
}

BLYNK_APP_DISCONNECTED() {
  // Your code here
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
    zblynk.write_enterCode();
    zstate.factory_reset();
    zblynk.updateAll();
    zblynk.debug_message("Hard Resetting");
    delay(500);
    System.reset();
  }
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  debug("Blynk is now connected - syncing all pins.\n");
  Blynk.syncAll();
  zblynk.updateAll();
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
      strcpy(zstate.p.vapeEmail, _email.c_str());
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
      strcpy(zstate.p.batteryEmail, _email.c_str());
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
      zblynk.write_zeroOffset();
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
BLYNK_WRITE(V27) {  zstate.p.enableBuzzerVape = param.asInt();  zstate.save();  }

// Update notifyVapor flag
BLYNK_WRITE(V28) {  zstate.p.enableNotifyVape = param.asInt();  zstate.save();  }

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
  int oldvalue = zstate.p.activityThreshold;

  switch (thresh) {
  case 1:	zstate.p.activityThreshold =  72;	break;
  case 3:	zstate.p.activityThreshold = 144;	break;
  default:	zstate.p.activityThreshold = 100;	break;
  }
  // If value changed since the acceleromter will need to be reinitalized save the new value and reset
  if (zstate.p.activityThreshold != oldvalue) {
    zstate.save();
    delay(4000);
    System.reset();
  }
}

