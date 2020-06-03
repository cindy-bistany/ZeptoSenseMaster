/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/util.ino"
#include "zstate.h"


//make a loop to go through the bit pattern parameter
//each bit gets a beep
//0 bits get one beep quantum, 1 bits get 3 beep quanta
//1 quantum between bits
//delete leading zeroes, replace with 6 beep quanta of quiet
//
//leading to morse code here eventually
//for now beep("000111000") or (beep"...---...") will produce morse SOS
//
void beep(string s);
void deepSleep(Zstate *st);
#line 13 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/util.ino"
#define dot_length 100 //ms sound for a morse dot
#define dash_length (3*dot_length)
#define char_space dash_length
#define word_space (7*dot_length)

#define buzzon() digitalWrite(buzzer, HIGH)
#define buzzoff() digitalWrite(buzzer, LOW)

void beep(string s) {
  for (int i=0; i<len(s); i++) {
    switch (s[i]) {
    '0':
    '.':
      buzzon(); delay(dot_length); buzzoff(); delay(dot_length);
      break;
    '1':
    '-':
      buzzon(); delay(dash_length); buzzoff(); delay(dot_length);
      break;
    ' ':
      buzzoff(); delay(dash_length); 
      break;
    default:
      break;
    }
  }
  buzzoff(); //belt & suspenders
}

void deepSleep(Zstate *st)
{
  shutdown_basehw(st);
  st->save());
  debug("Going to sleep\n");
  
  if (st->bSleepModeStandby){
    debug("Going to standby sleep\n");
    st->bInSleepMode=true;
    System.sleep(D8, RISING, 900, SLEEP_NETWORK_STANDBY);
    return;
  }
  else{
    debug("Going to deep sleep\n");
    st->bSleepModeStandby=false;
    st->bInSleepMode=true;
    delay(2000);
    System.sleep(SLEEP_MODE_DEEP); 
    }
}

