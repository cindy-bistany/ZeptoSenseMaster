#ifndef ZSTATE_H
#define ZSTATE_H

#include <stdint.h>

#define DEBUG 1

#ifdef DEBUG
#define debug(...) Serial1.print(__VA_ARGS__)
#define debugf(...)  Serial1.printf(__VA_ARGS__)
#else
#define debug(...)
#define debugf(...)
#endif


#define BUILD_RANDOM_NUMBER 31415; //a random number


//#####
// Zeptive config variables
//#####
class Zstate_persistent {
};

class Zstate {
 public:
  int build_random_number;  //to detect the very first power up after flashing
  bool terminalDebug;
  
  unsigned long wakeuptime, onTime, backhaulTime;

  int deviceTimeZone;	//0=America/New_York, 1=America/Chicago, 2=America/Phoenix, 3=America/Los_Angeles
  int gmtOffsetSeconds;
  bool timeIsSynchronized;	//to what?

  uint16_t portBlynk;	//probably should be hived off into blynk
  int portBuzzer;	//documented as D7, but what is D7?, maybe hive off into baseboard code

  char stateStr[64];
  
  int readingCount;	//some type of averaging I think
  int numberOfReadings; //something to do with tracking the readings while looping
  int secondsBetweenReadings;

  float zeroOffset;	//something for one of the sensors

  int activityThreshold;  // accelerometer 62.5mg per increment-activity thresholds (0-255) 36,72,100 or 144
  float batThreshold;  //something to do with charging the battery in the 2-layer battery arch
  
  char expression[256];  //must be the test expression for raining alerts
  char email[256];  //main email for alerts I think
  char batEmail[256];  //email for battery alerts
  char tamperEmail[256];  //email for tamper alerts
  
  bool lastAlert;  //guessing this is last vape alert
  bool batLastAlert;  //last battery alert, but so what?
  bool tamperLastAlert;  //last tamper alert, but so what?
 //bool buzzerActivated;

  //No sure how these have anything to do with pins.  certainly not here.
  //maybe these enable the respective alerts?
  /****************** New Blync Pins ******************/
  bool buzzerTamper; // Buzzer Tamper
  bool buzzerVapor; // Vapor Buzzer
  bool notifyVapor; // Vapor Notify
  bool notifyTamper; // Tamper Notify
  bool notifyBattery; // Battery Notify

  //Telcos don't like constant re/dis/connecting, and shut you off when it happens
  //This must be used to sleep for a while but keeping the phone on.
  bool bSleepModeStandby; // If TRUE will do a SLEEP_NETWORK_STANDBY if FALSE will do a SLEEP_MODE_DEEP

  //This I think was the state of the powered-almost-off mode.  But I also this should always be true, except first run.
  bool bInSleepMode; // If TRUE was in a sleep

  void factory_reset();
  void save();
  void load();
};

/*
  bool powerOn, appConnected, sensorValid, currentAlert,
    batCurrentAlert, tamperCurrentAlert;
*/
#endif
