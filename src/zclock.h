//
#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>

class Zclock {
 public:
  void setup();
  uint32_t now();
  void on(); //WWWWWWWWWWWWWWWhat is this

  void setGMTOffset(long offset);
  void setTimeZone(int tz);
  int getTimeZone();
  string timeZoneName();
  
 private:
  int deviceTimeZone;	//0=America/New_York, 1=America/Chicago, 2=America/Phoenix, 3=America/Los_Angeles
  int gmtOffsetSeconds;
  bool gmtOffsetValid;
  bool timeIsSynchronized;	//to what?
  void publishGMTOffsetRequest();

};

extern Zclock zclock;

#endif
