/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/state.ino"

#line 2 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/state.ino"
int firstRunValue = 31415; //a random number

// Structure to store state of device through power down cycles
typedef struct
{
  //to detect the very first power up after flashing
  //check against firstRunValue above
  int firstRunCheck;

  //something to do with tracking the readings while looping
  int numberOfReadings;
  int secondsBetweenReadings;

  //something for one of the sensors
  float zeroOff;

  //must be the test expression for raining alerts
  char expression[256];

  //main email for alerts I think
  char email[256];

  //email for battery alerts
  char batEmail[256];

  //email for tamper alerts
  char tamperEmail[256];

  //something to do with charging the battery in the 2-layer batytery arch
  float batThreshold;

  //guessing this is last vape alert
  bool lastAlert;

  //last battery alert, but so what?
  bool batLastAlert;

  //last tamper alert, but so what?
  bool tamperLastAlert;
 //bool buzzerActivated;

  //No sure how these have anything to do with pins.  certainly not here.
  //maybe these enable the respective alerts?
  /****************** New Blync Pins ******************/
  bool buzzerTamper; // Buzzer Tamper
  bool buzzerVapor; // Vapor Buzzer
  bool notifyVapor; // Vapor Notify
  bool notifyTamper; // Tamper Notify
  bool notifyBattery; // Battery Notify

  //same Q here
  int ActivityThreshold;  // 62.5mg per increment-activity thresholds (0-255) 36,72,100 or 144

  //weird way to do a time zone
  int deviceZone; //0=America/New_York, 1=America/Chicago, 2=America/Phoenix, 3=America/Los_Angeles

  //I think this is used for cellular.  telcos don't like constant re/dis/connecting, and shut you off when it happens
  //This must be used to sleep for a while but keeping th phone on.
  bool bSleepModeStandby; // If TRUE will do a SLEEP_NETWORK_STANDBY if FALSE will do a SLEEP_MODE_DEEP

  //This I think was the state of the powered-almost-off mode.  But I also thit should always be true, except first run.
  bool bInSleepMode; // If TRUE was in a sleep

  //I'm guessing this accumulates the operating time
  //Good for use in conjunction with other battery indicators.
  int OnTime;

} State;

void state_save(State st)
{
  // EEPROM.put(0, state);
  debug("Saving State\n");
  fram.put(0,st);
}

void state_load(State st)
{
  // EEPROM.get(0, state);
  debug("Loading State\n");
  fram.get(0,st);
}

void state_str(State st) {
  //print out the state struct on the debug serial port
}

