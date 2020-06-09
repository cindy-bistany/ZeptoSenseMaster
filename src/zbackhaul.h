//emacs -*- c++ -*-
#ifndef ZBACKHAUL_H
#define ZBACKHAUL_H

#include "zutil.h"
#include "zstate.h"
#include "zauth.h"

class Zbackhaul {
 public:
  void setup();
  void connect();
  void connectWithoutWaiting();
  bool isConnected();
  int signalStrength();
  
  void off();

 private:
  
};

extern Zbackhaul zbackhaul;

#endif
