//emacs -*- c++ -*-
#ifndef BACKHAUL_H
#define BACKHAUL_H

#include "util.h"
#include "zstate.h"
#include "auth.h"

class Backhaul {
 public:
  void connect();
  void connectWithoutWaiting();
  bool isConnected();

 private:
  
};

#endif
