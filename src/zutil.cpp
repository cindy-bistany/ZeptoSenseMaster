#include "zutil.h"
#include "zblynk.h"

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

void buzzon() { digitalWrite(BUZZER, HIGH); }
void buzzoff() { digitalWrite(BUZZER, LOW); }

void crash(char *msg)
{
  zblynk.debug_message(String("System resetting with condition %s\n") + msg);
  System.reset();
}

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


void beep(String s) {
  for (int i=0; i < s.length(); i++) {
    switch (s[i]) {
    case '0':
    case '.':
      buzzon(); delay(dot_length); buzzoff(); delay(dot_length);
      break;
    case '1':
    case '-':
      buzzon(); delay(dash_length); buzzoff(); delay(dot_length);
      break;
    case ' ':
      buzzoff(); delay(dash_length); 
      break;
    default:
      break;
    }
  }
  buzzoff(); //belt & suspenders
}


