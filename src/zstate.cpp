//emacs     -*- c++ -*-

#include <Wire.h>

#include "zbuild.h"
#include "zstate.h"
#include "zbackhaul.h"
#include "zbaseboard.h"
#include "zclock.h"
#include "zdetector.h"

Zstate zstate;

//Object for accessing persistent storage
#include <MB85RC256V-FRAM-RK.h>
MB85RC256V fram(Wire, 0);

void Zstate::setup() {
  load();
}

void Zstate::save()
{
  // EEPROM.put(0, state);
  debug("Saving State\n");
  fram.put(0,*this);
}

void Zstate::load()
{
  // EEPROM.get(0, state);
  debug("Loading State\n");
  fram.get(0,*this);
  if (p.build_random_number != BUILD_RANDOM_NUMBER) {  //Check for first time the firmware runs
    factory_reset();
    save();
  }
  else {
    debug("Not first run.\n");
    // firstRun =false;
  }

}

void Zstate::deepSleep()
{
  save();
  debug("Going to sleep\n");
  int sleepTime = 0;
#if Wiring_Cellular
  sleepTime = 900;
#endif
  zclock.deepSleep(sleepTime);
}

const String factory_expression = "pm1>300||pm2>300||pm4>300||pm10>300&&conc>100||temp>150";
const String factory_email	= "bistany@comcast.net";
const String factory_batEmail	= "bistany@comcast.net";
const String factory_tamperEmail= "bistany@comcast.net";

void Zstate::factory_reset()
{
  p.build_random_number = BUILD_RANDOM_NUMBER;
  
  p.deviceTimeZone = 0;		//America/New_York
  p.gmtOffsetSeconds = -4*3600;	//4 hours
  p.timeIsSynchronized = false;
  p.accumulatedOnTime = 0;
  
  p.portBlynk = 8080;
  p.portBuzzer = D7;

  p.wakeupTime = p.accumulatedOnTime = p.backhaulOnTime = 0;
  
  p.readingCount = 0;
  p.numberOfReadings = 24;
  p.secondsBetweenReadings = 5;
  p.zeroOffset = -156;

  strcpy(p.expression, factory_expression);
  strcpy(p.vapeEmail, factory_email);
  strcpy(p.batteryEmail, factory_batEmail);
  strcpy(p.tamperEmail, factory_tamperEmail);
  
  p.batteryThreshold = 20.0;
  p.lastAlert = false;
  p.batLastAlert = false;
  p.activityThreshold = 100; // Which is set to 1 in Blynk

  p.enableBuzzerTamper = true;  // Buzzer Tamper
  p.enableBuzzerVape = false; // Vapor Buzzer
  p.enableBuzzerBattery = false;
  
  p.enableNotifyVape = true; // Vapor Notify
  p.enableNotifyTamper = true; // Tamper Notify
  p.enableNotifyBattery = true; // Battery Notify

  p.terminalDebug = false;
  
  save();
}


String Zstate::str() {
  //print out the state struct on the debug serial port
}

