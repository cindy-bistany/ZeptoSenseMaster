//emacs -*- c++ -*-

#ifndef BASEBOARD_H
#define BASEBOARD_H

#include "zstate.h"

class Baseboard {
 public:
  void setup(Zstate *st);
  void shutdown(Zstate *st);

  void powerOnOff(int which, bool onoff);
  bool batteryIsCharged();
  float batteryLevel();
  float signalStrength();

 private:
  void setup_expander(Zstate *st);
  void setup_i2c(Zstate *st);
  void setup_i2c(Zstate *st);

#endif
