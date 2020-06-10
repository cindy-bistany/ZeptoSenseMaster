//emacs -*- c++ -*-
#ifndef ZBACKHAUL_H
#define ZBACKHAUL_H

#include "zutil.h"
#include "zstate.h"
#include "zauth.h"

class Zbackhaul {
 public:
  void setup();
  bool isConnected();
  bool isWifi();
  bool isCellular();
  int signalStrength();
  float batteryLevel();
  
  void shutdown();

 private:
  void connect();
  void connectWithoutWaiting();
  
};

extern Zbackhaul zbackhaul;

#endif
