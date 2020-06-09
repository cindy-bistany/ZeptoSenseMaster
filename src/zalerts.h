#ifndef ZALERTS_H
#define ZALERTS_H

#include "zdetector.h"

class Zalerts {
 public:
  void setup();
  void clear();
  int update();
  
  bool vapeAlert();
  bool batteryAlert();
  bool tamperAlert();
  
 private:
  bool vapeTrouble;
  bool batteryTrouble;
  bool tamperTrouble;

  bool runExpression(char * expr, Zdetector *zd);
};

extern Zalerts zalerts;

#endif
