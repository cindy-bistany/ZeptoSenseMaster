//emacs     -*- c++ -*-

#include <Wire.h>

#include "zbuild.h"
#include "zstate.h"
#include "zbackhaul.h"

Zstate zstate;

//Object for accessing persistent storage
#include <MB85RC256V-FRAM-RK.h>
MB85RC256V fram(Wire, 0);

void Zstate::setup() {
  factory_reset();
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
    backhaul.connect();
  }
  else {
    debug("Not first run.\n");
    // firstRun =false;
  }

}

void Zstate::deepSleep()
{
  save());
  zbaseboard.shutdown();
  debug("Going to sleep\n");
  
  if (bSleepModeStandby){
    debug("Going to standby sleep\n");
    bInSleepMode=true;
    System.sleep(D8, RISING, 900, SLEEP_NETWORK_STANDBY);
    return;
  }
  else{
    debug("Going to deep sleep\n");
    bSleepModeStandby=false;
    bInSleepMode=true;
    delay(2000);
    System.sleep(SLEEP_MODE_DEEP); 
    }
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

  p.wakeupTime = p.accumulatedOnTime = p.backhaulTime = 0;
  
  P.stateStr = "INIT";
  
  p.readingCount = 0;
  p.numberOfReadings = 24;
  p.secondsBetweenReadings = 5;
  p.zeroOffset = -156;
  strcpy(p.expression, factory_expression);
  strcpy(p.email, factory_email);
  strcpy(p.batEmail, factory_batEmail);
  strcpy(p.tamperEmail, factory_tamperEmail);
  p.batThreshold = 20.0;
  p.lastAlert = false;
  p.batLastAlert = false;
  p.activityThreshold = 100; // Which is set to 1 in Blynk
  p.buzzerTamper = true;  // Buzzer Tamper
  p.buzzerVapor = false; // Vapor Buzzer
  p.notifyVapor = true; // Vapor Notify
  p.notifyTamper = true; // Tamper Notify
  p.notifyBattery = true; // Battery Notify

  p.powerOn = p.appConnected = p.sensorValid = p.currentAlert =
    p.terminalDebug =  p.batCurrentAlert = p.tamperCurrentAlert;
  save();
}


str Zstate::str() {
  //print out the state struct on the debug serial port
}

