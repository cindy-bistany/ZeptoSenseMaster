// Particle.io (Arduino) code for zeptive vape sensor
// Copyright 2020 zeptive.com
//

#include "zstate.h"
#include "zbaseboard.h"
#include "zdetector.h"
#include "zclock.h"
#include "zbackhaul.h"
#include "zalerts.h"

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
  zbackhaul.setup();
  zdetector.setup();
  zalerts.setup();
}

void loop()
{
  zdetector.read();
  zalerts.loop();
  crash("test");
    
}

/*
//***************** INTERRUPT ******************
int interruptPin = A0;                 // Setup pin A0 for Boron

char *pm[5] = {"PM0.5", "PM1.0", "PM2.5", "PM4.0", "PM10"};

#define BLYNK_PRINT Serial1 // Defines the object that is used for printing
#define BLYNK_DEBUG        // Optional, this enables more detailed prints
#define BLYNK_HEARTBEAT      60



String StateString = "RDY";

BlynkTimer sensorTimer; // Create a Timer object called "sensorTimer"!
BlynkTimer accelTimer; // Create an accelerometer Timer object

*/
