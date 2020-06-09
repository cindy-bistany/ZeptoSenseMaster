// emacs -*- c++ -*-
#include <Particle.h>
#include <cstdlib>

////
//WARNING do not include zstate.h
////
//This class must have no dependencies on zstate
//That is, no permanent storage 
////

#include "zbuild.h"
#include "zutil.h"
#include "zclock.h"
#include "zbaseboard.h"
#include "zblynk.h"
#include "zbackhaul.h"

// Create new instance of RTC class:
// Use to hard power cycle the device to reset I2C
#include "MCP7941x.h"
MCP7941x rtc = MCP7941x();

Zclock zclock;

///
///callback for Particle clock sync event
///
void gmtOffsetHandler(const char *event, const char *data) {
  zclock.setGMTOffset(atoi(data));
}
///

void Zclock::deepSleep(long seconds)
{
  //seconds is the keepalive time for the cell modem
  if (seconds > 0) System.sleep(D8, RISING, seconds, SLEEP_NETWORK_STANDBY);
  else {
    zbaseboard.shutdown();
    zbackhaul.off();
    System.sleep(SLEEP_MODE_DEEP);
  }
}

void Zclock::setup()
{
  //If we woke up from the clock, that's because we left the cell modem on
  //we can turn it off now
  
  SleepResult result = System.sleepResult();
  switch (result.reason()) {
  case WAKEUP_REASON_NONE:       break;
  case WAKEUP_REASON_PIN:        break;
  case WAKEUP_REASON_RTC:        deepSleep(0); break;
  case WAKEUP_REASON_PIN_OR_RTC: break;
  default: break;
  }

  // Sync time if needed
  if (Time.isValid() && !timeIsSynchronized) {
    unsigned long now = Time.now();
    rtc.setUnixTime(now);

    unsigned long rtcnow = rtc.rtcNow();
    unsigned long diff = rtcnow - now;
    if (diff < 0) diff = -diff;
    if (diff < 10) {
      timeIsSynchronized = true;
      debug("Time is synced to the cloud\n"); 
    }
  }
  
  long int jan_1_2000 = 946684800;
  long int jan_1_2099 = 4102444799;
  long int clockTime = rtc.rtcNow();
  if (clockTime<jan_1_2000 || clockTime>jan_1_2099) {
    rtc.setUnixTime(BUILD_NUMBER);
    timeIsSynchronized = false;
  }
  Particle.subscribe("hook-response/gmtOffset", gmtOffsetHandler, MY_DEVICES);
  if (Particle.connected() && !gmtOffsetValid) 
    Particle.publish("gmtOffset", zclock.timeZoneName().c_str(), PRIVATE);
}

uint32_t Zclock::now() {
  if (Particle.connected() && !gmtOffsetValid) 
    Particle.publish("gmtOffset", zclock.timeZoneName().c_str(), PRIVATE);
  return rtc.rtcNow();
}


void Zclock::setTimeZone(int tz) { deviceTimeZone = tz; }
int Zclock::getTimeZone() { return deviceTimeZone; }

void Zclock::setGMTOffset(long offset)
{
  gmtOffsetSeconds = offset;
  gmtOffsetValid = true;
}

String Zclock::timeZoneName()
{
  String zone;
  switch (zclock.getTimeZone()) {
  case 0:    zone = "America/New_York";    break;
  case 1:    zone = "America/Chicago";    break;
  case 2:    zone = "America/Phoenix";    break;
  case 3:    zone = "America/Los_Angeles";    break;
  default:   zone = "America/New_York";    break;
  }
  return zone;
}

/*
void Zclock::timerSleep(long seconds)
{
  String msg =  String(Time.format(now(), "%h%e %R")) + " " + zbaseboard.batteryLevel() + "%";
  zblynk.status_message(msg);

  int alarmTime = rtc.rtcNow() + seconds;
  rtc.disableAlarm0();
  rtc.setAlarm0UnixTime(alarmTime);
  rtc.clearIntAlarm0();
 
  // Enable the alarm. This will set the MFP output low
  // turning off the power until the alarm triggers
  // turning on the power again and starting the code from
  // the beginning.
  rtc.enableAlarm0();
  rtc.publishAlarm0Debug();

  delay(200);  
}
*/

/*
this was never called in the original code

void initializeAlarm()
{
  debug("Initializing alarm\n");
  rtc.outHigh();
  rtc.disableAlarms();
  rtc.maskAlarm0("Seconds");
  rtc.setAlarm0PolHigh();
  rtc.clearIntAlarm0();
}
*/

