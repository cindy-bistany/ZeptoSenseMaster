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

void loop_clock()
{
}

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

