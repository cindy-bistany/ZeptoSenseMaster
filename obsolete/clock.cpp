/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/clock.ino"
// emacs -*- c++ -*-
#include "zstate.h"
#include "clock.h"
#include "MCP7941x.h"

// Create new instance of RTC class:
// Use to hard power cycle the device to reset I2C
void setup_clock(Zstate *st);
void loop_clock();
uint32_t clocknow();
void clockon();
void gmtOffsetHandler(const char *event, const char *data);
void publishGMTOffsetRequest();
#line 8 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/clock.ino"
MCP7941x rtc = MCP7941x();

void setup_clock(Zstate *st)
{  
  // Sync time if needed
  if (Time.isValid()&&!timeSynced)
  {
    rtc.setUnixTime(Time.now());
    if (abs(Time.now()-rtc.rtcNow())<10)
    {
      timeSynced=true;
      debug("Time is synced to the cloud\n"); 
      /*String TimeMsg;
      TimeMsg="Time "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R");
      debug(TimeMsg); + "\n"*/
    }
  }
}

void loop_clock()
{
}

uint32_t clocknow() { return rtc.rtcNow(); }

void clockon()
{
  long int clockTime = rtc.rtcNow();
  debug("Before\n");
  debug(clockTime);
  debug(": \n");
  debug(String(Time.format(clockTime, TIME_FORMAT_ISO8601_FULL)) + "\n");
  if (clockTime<946684800||clockTime>4102444799)
    {
      // 2019-01-01T00:00:00+00:00 in ISO 8601
      // Actual time is not important for rtc reset but needs to be a positive unix time
      rtc.setUnixTime(1262304000);
      long int clockTime = rtc.rtcNow();
      debug("After\n");
      debug(clockTime);
      debug(": \n");
      debug(String(Time.format(clockTime, TIME_FORMAT_ISO8601_FULL)) + "\n");
    }
  timeSynced=false;
}

void gmtOffsetHandler(const char *event, const char *data) {
  // Handle the integration response
  gmtOffsetSeconds=atoi(data);
  gmtOffsetValid = true;
  debug("GMT seconds offset is: ");
  debug(gmtOffsetSeconds + "\n");
}

void publishGMTOffsetRequest()
{
  // Send to https://timezonedb.com webhook for gmtOffset
  debug("publishGMTOffsetRequest Device Zone value is: ");
  debug(state.deviceZone + "\n");
  switch (state.deviceZone)
  {
    case 0:
      // America/New_York
      Particle.publish("gmtOffset", "America/New_York", PRIVATE);
      break;
    case 1:
      // America/Chicago
      Particle.publish("gmtOffset", "America/Chicago", PRIVATE);
      break;
    case 2:
      // America/Phoenix
      Particle.publish("gmtOffset", "America/Phoenix", PRIVATE);
      break;
    case 3:
      // America/Los_Angeles
      Particle.publish("gmtOffset", "America/Los_Angeles", PRIVATE);
      break;
    default:
      // America/New_York
      Particle.publish("gmtOffset", "America/New_York", PRIVATE);  
  }
}


