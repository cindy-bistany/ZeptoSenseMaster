// Particle.io (Arduino) code for zeptive vape sensor
// Copyright 2020 zeptive.com
//

#include "zstate.h"
#include "zbaseboard.h"
#include "zdetector.h"
#include "zclock.h"
#include "zbackhaul.h"
#include "zalerts.h"

/*
Wake up.
If we woke from timer that was set due to inactivity, the clock setup will put us back to sleep PIR-awaiting sleep.
Otherwise we continue.
Restore or initialize the system state.
Start the baseboard, backhaul, detector, and alerts.
 */
void setup()
{
  //just waking up from power off
  unsigned long wakeupTime = millis();	//note what time we woke up
  //must do this first b/c if woken up from snooze, we may just want to go back to lower power sleep
  zclock.setup();
  
  //evidently we didn't decide to go back to sleep
  zstate.setup();
  zstate.p.wakeupTime = wakeupTime;
  zbaseboard.setup();
  zbaseboard.morse("OK");
  zbackhaul.setup();
  zbaseboard.morse("BH OK");
  zdetector.setup();
  zbaseboard.beep("ZD OK");
  zalerts.setup();
  zbaseboard.beep("ZA OK");
}

/*
Outer loop for 2 minutes.
Inner loop for 5 seconds, taking a reading each time.
 */
const long readingInterval = zstate.p.secondsBetweenReadings*1000;
const long stayupTime = zstate.p.numberOfReadings * readingInterval;

void loop()
{
  unsigned long now = millis();
  
  unsigned long elapsedTime = now - zstate.p.wakeupTime;
  if (elapsedTime > stayupTime) zstate.sleep();

  elapsedTime = now - zdetector.lastReadingTime();
  if (elapsedTime > readingInterval) {
    zdetector.read();
    zalerts.loop();
  }
}

/*
//***************** INTERRUPT ******************
int interruptPin = A0;                 // Setup pin A0 for Boron

char *pm[5] = {"PM0.5", "PM1.0", "PM2.5", "PM4.0", "PM10"};


*/
