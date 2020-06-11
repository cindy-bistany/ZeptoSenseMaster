#include "zutil.h"
#include "zblynk.h"
#include "zbaseboard.h"

// EXAMPLE - defining and using a LED status
LEDStatus blinkYellow(RGB_COLOR_YELLOW, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);
LEDStatus blinkRed(RGB_COLOR_RED, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

void LEDPanic()
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

void crash(int code)
{
  LEDPanic();
  zblynk.debug_message(String("System crash: code ") + String(code) + "\n");
  zbaseboard.horn(code);
  System.reset();
}

void crash(char *msg)
{
  LEDPanic();
  zblynk.debug_message(String("System crash: ") + msg + "\n");
  zbaseboard.horn(HORN_CRASH_SEETEXT);
  System.reset();
}


