// emacs -*- c++ -*-
#include <Particle.h>
#include <cstdlib>

#include "util.h"
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
  Particle.subscribe("hook-response/gmtOffset", gmtOffsetHandler, MY_DEVICES);
  if (Particle.connected() && !gmtOffsetValid) 
    Particle.publish("gmtOffset", zclock.timeZoneName().c_str(), PRIVATE);
}

uint32_t Zclock::now() {
  if (Particle.connected() && !gmtOffsetValid) 
    Particle.publish("gmtOffset", zclock.timeZoneName().c_str(), PRIVATE);
  return rtc.rtcNow();
}

void Zclock::on()
{
  ///////////WWWWWWWWWWWWWWWWWWWWWWhat is this for
  long int clockTime = rtc.rtcNow();
  if (clockTime<946684800||clockTime>4102444799) {
      // 2019-01-01T00:00:00+00:00 in ISO 8601
      // Actual time is not important for rtc reset but needs to be a positive unix time
      rtc.setUnixTime(1262304000);
    }
  timeIsSynchronized = false;
}

void Zclock::setTimeZone(int tz) { deviceTimeZone = tz; }
int Zclock::getTimeZone() { return deviceTimeZone; }

void Zclock::setGMTOffset(long offset)
{
  gmtOffsetSeconds = offset;
  gmtOffsetValid = true;
}

string Zclock::timeZoneName()
{
  string zone;
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




