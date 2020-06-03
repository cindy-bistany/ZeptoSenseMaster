/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/zstate.ino"
//emacs     -*- c++ -*-

//Object for accessing persistent storage
#include <MB85RC256V-FRAM-RK.h>
#line 5 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/zstate.ino"
MB85RC256V fram(Wire, 0);

int firstRunValue = 31415; //a random number

Zstate::Zstate() {
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
  if (build_random_number != BUILD_RANDOM_NUMBER) {  //Check for first time the firmware runs
    factory_reset();
    connect();
  }
  else {
    debug("Not first run.\n");
    // firstRun =false;
  }

}

void Zstate::factory_reset()
{
    // Set factory defaults
  build_random_number = BUILD_RANDOM_NUMBER;
  deviceZone = 0; //America/New_York
  portBlynk = 8080;
  portBuzzer = D7;

  wakeupTime = onTime = backhaulTime = 0;
  
  stateStr = "INIT";
  
  readingCount = 0;
  numberOfReadings = 24;
  secondsBetweenReadings = 5;
  zeroOff = -156;
  strcpy(expression,"pm1>300||pm2>300||pm4>300||pm10>300&&conc>100||temp>150");
  strcpy(email,"bistany@comcast.net");
  strcpy(batEmail,"bistany@comcast.net");
  strcpy(tamperEmail,"bistany@comcast.net");
  batThreshold = 20.0;
  lastAlert = false;
  batLastAlert = false;
  ActivityThreshold = 100; // Which is set to 1 in Blynk
  buzzerTamper = true;  // Buzzer Tamper
  buzzerVapor = false; // Vapor Buzzer
  notifyVapor = true; // Vapor Notify
  notifyTamper = true; // Tamper Notify
  notifyBattery = true; // Battery Notify
  OnTime = 0;
  save();
}


void Zstate::str() {
  //print out the state struct on the debug serial port
}

void Zreadings::clear() {
  // Flag to keep track of accel interrupts
  accelInterrupt = false;
  for (int i=0; i<4; i++) mass_concen[i] = 0.0;
  for (int i=0; i<5; i++) num_concen[5] = 0.0;
  powerOn = appConnected = sensorValid = currentAlert =
    terminalDebug =  batCurrentAlert = tamperCurrentAlert;
}

//#####
// Zeptive alerts
//#####
typedef struct {
} Zalerts;

