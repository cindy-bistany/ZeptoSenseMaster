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
#define dot_length 100 //ms sound for a morse dot
#define dash_length (3*dot_length)
#define char_space dash_length
#define word_space (7*dot_length)

#define buzzon() digitalWrite(buzzer, HIGH)
#define buzzoff() digitalWrite(buzzer, LOW)

// EXAMPLE - defining and using a LED status
LEDStatus blinkYellow(RGB_COLOR_YELLOW, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);
LEDStatus blinkRed(RGB_COLOR_RED, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

void blinkpanic()
{
    blinkRed.setActive(true);
    delay(3000);
    blinkRed.setActive(false);
    blinkYellow.setActive(true);
    delay(3000);
    blinkYellow.setActive(false);
    blinkRed.setActive(true);
    delay(3000);
}


void blynk_status_message(String msg)
{
  Blynk.virtualWrite(V30, msg);
}

void blynk_debug_message(String msg)
{
  Blynk.virtualWrite(V21, msg);
}

void beep(String s) {
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

