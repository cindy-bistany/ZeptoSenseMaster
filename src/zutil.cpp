#include "zutil.h"
#include "zblynk.h"
#include "zbaseboard.h"

void crash(char *msg)
{
  zblynk.debug_message(String("System crash: ") + msg + "\n");
  zbaseboard.beep("...---...  ...---...  ...---...");
  System.reset();
}

void reboot(char *msg)
{
  zblynk.debug_message(String("System reboot: ") + msg + "\n");
  System.reset();
}

// EXAMPLE - defining and using a LED status
LEDStatus blinkYellow(RGB_COLOR_YELLOW, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);
LEDStatus blinkRed(RGB_COLOR_RED, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

void panic(char *msg)
{
  // System.reset();
  zblynk.debug_message(String("System panic: ") + msg + "\n");
  zbaseboard.beep("........");
  blinkRed.setActive(true);
  delay(3000);
  blinkRed.setActive(false);
  blinkYellow.setActive(true);
  delay(3000);
  blinkYellow.setActive(false);
  blinkRed.setActive(true);
  delay(3000);
  //System.reset();
  while (true)
    ;
}




