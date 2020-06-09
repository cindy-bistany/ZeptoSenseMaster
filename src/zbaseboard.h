//emacs -*- c++ -*-

#ifndef ZBASEBOARD_H
#define ZBASEBOARD_H

#include <IoTNodePower.h>
#include "zstate.h"

class Zbaseboard {
 public:
  void setup();
  void shutdown();

  void power3(bool onoff);
  void power5(bool onoff);
  bool power3IsOn();
  bool power5IsOn();
  bool batteryIsCharged();
  float batteryLevel();
  int signalStrength();

  void buzzer(bool onoff);

 private:
  void setup_expander();
  void setup_i2c();
  IoTNodePower boardPower;
  bool v5IsOn;
  bool v3IsOn;
};

extern Zbaseboard zbaseboard;

#endif
