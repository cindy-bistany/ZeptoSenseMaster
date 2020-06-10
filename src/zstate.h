#ifndef ZSTATE_H
#define ZSTATE_H

#include <stdint.h>
#include "application.h"

using namespace std;


//#####
// Zeptive config variables
//#####
typedef struct Zstate_persistent { //struct for easy storage to FRAM
 public:
  unsigned long build_number;	//serialized build number
  int build_random_number;	//to detect the very first power up after flashing
  bool terminalDebug;

  int deviceTimeZone;
  long int gmtOffsetSeconds;
  bool gmtOffsetValid;
  bool timeIsSynchronized;
  
  unsigned long wakeupTime, accumulatedOnTime, backhaulOnTime;

  int readingCount;	//some type of averaging I think
  int numberOfReadings; //something to do with tracking the readings while looping
  int secondsBetweenReadings;

  float zeroOffset;	//something for one of the sensors

  int activityThreshold;  // accelerometer 62.5mg per increment-activity thresholds (0-255) 36,72,100 or 144
  float batteryThreshold;  //something to do with charging the battery in the 2-layer battery arch
  
  char expression[256];  //must be the test expression for raining alerts
  char vapeEmail[256];  //main email for alerts I think
  char batteryEmail[256];  //email for battery alerts
  char tamperEmail[256];  //email for tamper alerts
  
  bool enableBuzzerVape;	// Vape Buzzer
  bool enableBuzzerTamper;	// Buzzer Tamper
  bool enableBuzzerBattery;
  
  bool enableNotifyVape;	// Vape Notify
  bool enableNotifyTamper;	// Tamper Notify
  bool enableNotifyBattery;	// Battery Notify

} Zstate_persistent;

class Zstate {
 public:
  Zstate_persistent p;

  void setup();
  void factory_reset();
  void save();
  void load();
  String str();
  void sleep();
};

/*
  bool powerOn, appConnected, sensorValid, currentAlert,
    batCurrentAlert, tamperCurrentAlert;
*/

extern Zstate zstate;

#endif
