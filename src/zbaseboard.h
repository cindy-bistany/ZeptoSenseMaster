//emacs -*- c++ -*-

#ifndef ZBASEBOARD_H
#define ZBASEBOARD_H

#include "zstate.h"

class Baseboard {
 public:
  void setup();
  void shutdown();

  void powerOnOff(int which, bool onoff);
  int powerIsOnOff();
  bool batteryIsCharged();
  float batteryLevel();
  float signalStrength();

  void buzzer(bool onoff);

 private:
  void setup_expander();
  void setup_i2c();
  IoTNodePower boardPower;
  bool v5IsOn;
  bool v3IsOn;
};

#endif
