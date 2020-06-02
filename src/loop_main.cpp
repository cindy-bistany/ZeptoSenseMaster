/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/loop_main.ino"
//called while we are powered on

void loop();
#line 3 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/loop_main.ino"
void loop()
{
   //If woken up by a RTC Sleep then was in Standby so go into Deep Sleep now
  SleepResult result = System.sleepResult();
  switch (result.reason()) {
    case WAKEUP_REASON_NONE: {
      break;
    }
    case WAKEUP_REASON_PIN: {
      break;
    }
    case WAKEUP_REASON_RTC: {
      debug("Device was woken up by the Particle RTC (after 15 minutes), go into Deep Sleep\n");
      state.bSleepModeStandby=false;
      // Delay here in loop is okay because we are about to sleep
      digitalWrite(buzzer, LOW);
      delay(2000);
      deepSleep();
      break;
    }
    case WAKEUP_REASON_PIN_OR_RTC: {
      break;
    }
  }

  Blynk.run();
  // Note the use of timers in the loop per Blynk best practice
  sensorTimer.run(); // BlynkTimer is working...
  accelTimer.run();

  if (readingCount>=state.numberOfReadings)// Number of readings variable
  {
    // Delay here in loop is okay because we are about to sleep
    digitalWrite(buzzer, LOW);
    delay(2000);
    deepSleep();
  }

}
