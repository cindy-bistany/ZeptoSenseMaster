#ifndef ZBLYNK_H
#define ZBLYNK_H

#include <string>
using namespace std;

#include <blynk.h>
#include "zstate.h"

class Zblynk {
 public:
  void init(Zstate *st);

  void status_message(string msg);
  void debug_message(string msg);

  void setAppWidgets(Zstate *st);
  void initializeAppWidgets(Zstate *st);

 private:
  Zstate *st;

};

extern Zblynk zblynk;

#endif
