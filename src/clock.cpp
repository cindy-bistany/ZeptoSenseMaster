/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/clock.ino"
// Create new instance of RTC class:
// Use to hard power cycle the device to reset I2C
void setup_clock();
#line 3 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/clock.ino"
MCP7941x rtc = MCP7941x();

void setup_clock()
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

