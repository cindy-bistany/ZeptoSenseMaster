//emacs -*- c++ -*-
#ifndef ZBACKHAUL_H
#define ZBACKHAUL_H

#include "zutil.h"
#include "zstate.h"
#include "zauth.h"

class Backhaul {
 public:
  void connect();
  void connectWithoutWaiting();
  bool isConnected();

 private:
  
};

Backhaul backhaul;

#endif
