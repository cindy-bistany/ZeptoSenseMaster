#include <MB85RC256V-FRAM-RK.h>

int firstRunValue = 31415; //a random number

typedef struct {
  uint16_t portBlynk;
} ZBlynkconfig;

void zblynkconfig_clear(ZBlynk *zbc)
{
  zbc->portBlynk = 8080;
}

typedef struct {

  int firstRunCheck;  //to detect the very first power up after flashing
  //check against firstRunValue above

  int numberOfReadings;  //something to do with tracking the readings while looping
  int secondsBetweenReadings;

  float zeroOff;  //something for one of the sensors
  char expression[256];  //must be the test expression for raining alerts
  char email[256];  //main email for alerts I think
  char batEmail[256];  //email for battery alerts
  char tamperEmail[256];  //email for tamper alerts
  float batThreshold;  //something to do with charging the battery in the 2-layer battery arch
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

  //same Q here
  int ActivityThreshold;  // 62.5mg per increment-activity thresholds (0-255) 36,72,100 or 144

  //weird way to do a time zone
  int deviceZone; //0=America/New_York, 1=America/Chicago, 2=America/Phoenix, 3=America/Los_Angeles

  //I think this is used for cellular.  telcos don't like constant re/dis/connecting, and shut you off when it happens
  //This must be used to sleep for a while but keeping the phone on.
  bool bSleepModeStandby; // If TRUE will do a SLEEP_NETWORK_STANDBY if FALSE will do a SLEEP_MODE_DEEP

  //This I think was the state of the powered-almost-off mode.  But I also this should always be true, except first run.
  bool bInSleepMode; // If TRUE was in a sleep

  //I'm guessing this accumulates the operating time
  //Good for use in conjunction with other battery indicators.
  int OnTime;

} Zconfig;

void zconfig_clear(ZBlynk *zbc) {
  zbc->firstRunCheck = 0;

  zbc->numberOfReadings = 15;
  zbc->secondsBetweenReadings=5;
  zbc->readingCount = 0;

  zbc->zeroOff = 0;
  strcpy(zbc->expression,"Enter expression here");
  strcpy(zbc->email,"Enter email here");
  strcpy(zbc->batEmail,"Enter email here");
  strcpy(zbc->tamperEmail,"Enter email here");
  
  zbc->batThreshold = 20;
  zbc->deviceZone = 0;
  zbc->lastAlert = false;
  zbc->batLastAlert = false;
  zbc->buzzerTamper = true;  // Buzzer Tamper
  zbc->buzzerVapor = false; // Vapor Buzzer
  zbc->notifyVapor = true; // Vapor Notify
  zbc->notifyTamper = true; // Tamper Notify
  zbc->notifyBattery = true; // Battery Notify

  zbc->ActivityThreshold = 100; // Which is set to 1 in Blynk
  zbc->OnTime = 0;

  zbc->CycleOnTime = 0;
  zbc->VapeAlertTime = 0;
  zbc->VapeBuzzerOn = false;
  zbc->VapeAlertBuzzerTime = 0;

  zbc->buzzer = D7;
  zbc->gmtOffsetSeconds = -14400;
  zbc->gmtOffsetValid = false;
  zbc->timeSynced = false;
  zbc->bSleepModeStandby=true;
  zbc->bInSleepMode=false;
}

typedef struct {
  // Flag to keep track of accel interrupts
  bool accelInterrupt;
  float mass_concen[4];
  float num_concen[5];
  bool powerOn, appConnected, sensorValid, currentAlert,
    terminalDebug, batCurrentAlert, tamperCurrentAlert;
} Zreadings;

void zreadings_clear(Zreadings *zrd) {
  // Flag to keep track of accel interrupts
  zrd->accelInterrupt = false;
  for (int i=0; i<4; i++) zrd->mass_concen[i] = 0.0;
  for (int i=0; i<5; i++) zrd->num_concen[5] = 0.0;
  zrd->powerOn = zrd->appConnected = zrd->sensorValid = zrd->currentAlert =
    zrd->terminalDebug =  zrd->batCurrentAlert = zrd->tamperCurrentAlert;
}

typedef struct {
} Zalerts;

typedef Zstate {
  ZBlinkconfig zbc;
  Zreadings zrd;
  Zalerts zal;
} Zstate;  

void zstate_save(Zstate *st)
{
  // EEPROM.put(0, state);
  debug("Saving State\n");
  fram.put(0,*st);
}

void zstate_load(Zstate *st)
{
  // EEPROM.get(0, state);
  debug("Loading State\n");
  fram.get(0,*st);
}

void zstate_init(Zstate st)
{
    // Set factory defaults
    st->firstRunCheck = firstRunValue;
    st->numberOfReadings = 24;
    st->secondsBetweenReadings = 5;
    st->zeroOff = -156;
    strcpy(st->expression,"pm1>300||pm2>300||pm4>300||pm10>300&&conc>100||temp>150");
    strcpy(st->email,"bistany@comcast.net");
    strcpy(st->batEmail,"bistany@comcast.net");
    strcpy(st->tamperEmail,"bistany@comcast.net");
    st->batThreshold = 20.0;
    st->lastAlert = false;
    st->batLastAlert = false;
    st->ActivityThreshold = 100; // Which is set to 1 in Blynk
    st->buzzerTamper = true;  // Buzzer Tamper
    st->buzzerVapor = false; // Vapor Buzzer
    st->notifyVapor = true; // Vapor Notify
    st->notifyTamper = true; // Tamper Notify
    st->notifyBattery = true; // Battery Notify
    st->OnTime = 0;
    st->deviceZone=0; //America/New_York
    zstate_save();
}


void state_restore(ZState *st)
{
  state_load(st);
  if (st->firstRunCheck != firstRunValue) {  //Check for first time the firmware runs
    state_init(st);
    connect();
  }
  else {
    debug("Not first run.\n");
    // firstRun =false;
  }

}

void state_str(State st) {
  //print out the state struct on the debug serial port
}

