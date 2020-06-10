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
  float batteryLevel();

  void buzzer(bool onoff);
  void beep(String dotdash);

 private:
  void setup_expander();
  void setup_i2c();
  IoTNodePower boardPower;
  bool v5IsOn;
  bool v3IsOn;
};

extern Zbaseboard zbaseboard;

#endif
