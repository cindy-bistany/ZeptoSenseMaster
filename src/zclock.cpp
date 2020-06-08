// emacs -*- c++ -*-
#include <Particle.h>
#include <cstdlib>

#include "zbuild.h"
#include "zutil.h"
#include "zclock.h"

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

void Zclock::setup()
{  
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
  case 0:
    zone = "America/New_York";
    break;
  case 1:
    zone = "America/Chicago";
    break;
  case 2:
    zone = "America/Phoenix";
    break;
  case 3:
    zone = "America/Los_Angeles";
    break;
  default:
    zone = "America/New_York";
    break;
  }
  return zone;
}




