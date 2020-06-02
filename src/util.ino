#define SERIAL_DEBUG
#define KURTDEBUG

#ifdef SERIAL_DEBUG
  #define debug(...) Serial1.print(__VA_ARGS__)
  #define debugf(...) Serial1.printf(__VA_ARGS__)
#else
  #define debug(...)
  #define debugf(...)
#endif


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
}

void deepSleep(zState *st)
{
  shutdown_basehw(st);
  state_save(st);
  debug("Going to sleep\n");
  
  if (state.bSleepModeStandby){
    debug("Going to standby sleep\n");
    state.bInSleepMode=true;
    System.sleep(D8, RISING, 900, SLEEP_NETWORK_STANDBY);
    return;
  }
  else{
    debug("Going to deep sleep\n");
    state.bSleepModeStandby=false;
    state.bInSleepMode=true;
    delay(2000);
    System.sleep(SLEEP_MODE_DEEP); 
    }
}

