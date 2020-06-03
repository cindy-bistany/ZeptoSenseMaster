// Particle.io (Arduino) code for zeptive vape sensor
// Copyright 2020 zeptive.com
//

//ideally:
ZState currentState;

setup()
{
  //just waking up from power off
  unsigned long wakeupTime = millis();	//note what time we woke up
  
  restore_state(&currentState);		//restore prev or init new state
  currentState.wakeupTime = wakeupTime;	//note the original wake up time.
  
  setup_basehw(&currentState);
  setup_clock(&currentState);
  setup_protection(&currentState);
  setup_detection(&currentState);
  setup_alerts(&currentState);
  setup_backhaul(&currentState);
}

loop()
{
   //If woken up by a RTC Sleep then was in Standby so go into Deep Sleep now
  SleepResult result = System.sleepResult();
  switch (result.reason()) {
    case WAKEUP_REASON_NONE: {
      break;
    }
    case WAKEUP_REASON_PIN: {
      break;
    }
    case WAKEUP_REASON_RTC: {
      debug("Device was woken up by the Particle RTC (after 15 minutes), go into Deep Sleep\n");
      state.bSleepModeStandby=false;
      // Delay here in loop is okay because we are about to sleep
      digitalWrite(buzzer, LOW);
      delay(2000);
      deepSleep();
      break;
    }
    case WAKEUP_REASON_PIN_OR_RTC: {
      break;
    }
  }

  Blynk.run();
  // Note the use of timers in the loop per Blynk best practice
  sensorTimer.run(); // BlynkTimer is working...
  accelTimer.run();

  if (readingCount>=state.numberOfReadings)// Number of readings variable
  {
    // Delay here in loop is okay because we are about to sleep
    digitalWrite(buzzer, LOW);
    delay(2000);
    deepSleep();
  }
}

#include "MCP7941x.h"


//***************** INTERRUPT ******************
int interruptPin = A0;                 // Setup pin A0 for Boron

#ifdef PARTICLE
#else
  #include "arduino.h"
  #include <Wire.h>  
#endif //end of #ifdef PARTICLE

#define Version_2


#ifdef Version_2
#define alertRedImage 1
#define alertGreenImage 0
#else
#define alertRedImage 2
#define alertGreenImage 1
#endif

// SYSTEM_MODE(SEMI_AUTOMATIC);
// SYSTEM_THREAD(ENABLED);

// EXAMPLE - defining and using a LED status
LEDStatus blinkYellow(RGB_COLOR_YELLOW, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);
LEDStatus blinkRed(RGB_COLOR_RED, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);


char *pm[5] = {"PM0.5", "PM1.0", "PM2.5", "PM4.0", "PM10"};

#define BLYNK_PRINT Serial1 // Defines the object that is used for printing
#define BLYNK_DEBUG        // Optional, this enables more detailed prints
#define BLYNK_HEARTBEAT      60



String StateString = "RDY";

BlynkTimer sensorTimer; // Create a Timer object called "sensorTimer"!
BlynkTimer accelTimer; // Create an accelerometer Timer object

///////////////////////

readhw();

// Checks if alert status changed and resets lastAlert if it has
bool alertChanged(Zstate *st)
{
  if (st->currentAlert == st->lastAlert)
    return false;
  st->lastAlert = currentAlert;
  saveState(st);
  return true;
}

// Checks if alert status changed and resets lastAlert if it has
bool batAlertChanged(Zstate *st)
{
  if (st->batCurrentAlert == st->batLastAlert)
    return false;
  st->batLastAlert = batCurrentAlert;
  saveState(st);
  return true;
}

// Checks if tamper alert status changed 
bool tamperAlertChanged(Zstate *st)
{
  if (st->tamperCurrentAlert == st->tamperLastAlert)
    return false;
  st->tamperLastAlert = tamperCurrentAlert;
  saveState(st);
  return true;
}

void timerSleep(long seconds)
{
  String statusMessage;
  StateString = "STBY";
  statusMessage = timeSynced ?
    StateString + " " + Time.format(rtc.rtcNow() + gmtOffsetSeconds,"%h%e %R") + " " + field7 + "%"
    : StateString + "                " + field7 + "%";
  Blynk.virtualWrite(V30,statusMessage);
  setAlarm(seconds);
  activateAlarmPowerdown();
  delay(200);  
}


void setAlarm(int DURATION)
{
    rtc.disableAlarm0();
    int rtcnow = rtc.rtcNow();
    debug(rtcnow + "\n");
    int alarmTime = rtcnow + DURATION;
    String mess = String(Time.format(rtcnow, TIME_FORMAT_ISO8601_FULL))+" + "+String(DURATION)+" = "+String(Time.format(alarmTime, TIME_FORMAT_ISO8601_FULL))+"\r\n";
    debug(mess);
    if (terminalDebug) Blynk.virtualWrite(V21, mess);
    rtc.setAlarm0UnixTime(alarmTime);
}

void initializeAlarm()
{
  debug("Initializing alarm\n");
  rtc.outHigh();
  rtc.disableAlarms();
  rtc.maskAlarm0("Seconds");
  rtc.setAlarm0PolHigh();
  rtc.clearIntAlarm0();
}

void activateAlarmPowerdown()
{
    // Clear the alarm0 interrupt
    rtc.clearIntAlarm0();
 
    // Enable the alarm. This will set the MFP output low
    // turning off the power until the alarm triggers
    // turning on the power again and starting the code from
    // the beginning.
    rtc.enableAlarm0();
 rtc.publishAlarm0Debug();
    
}
