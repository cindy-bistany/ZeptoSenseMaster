#ifndef ZBLYNK_H
#define ZBLYNK_H

#include "application.h"

#include <blynk.h>
#include "zstate.h"

class Zblynk {
 public:
  void init(Zstate *st);

  void status_message(String msg);
  void debug_message(String msg);

  void setAppWidgets(Zstate *st);
  void initializeAppWidgets(Zstate *st);

 private:
  Zstate *st;

};

extern Zblynk zblynk;

#endif
