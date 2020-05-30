// Create new instance of RTC class:
// Use to hard power cycle the device to reset I2C
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

