#ifndef ZALERTS_H
#define ZALERTS_H

#include "zdetector.h"

class Zalerts {
 public:
  void setup();
  void clear();
  void loop();
  int update();
  
  bool vapeAlert();
  bool batteryAlert();
  bool tamperAlert();
  
 private:
  bool vapeTrouble, vapeTrouble_prev;
  bool batteryTrouble, batteryTrouble_prev;
  bool tamperTrouble, tamperTrouble_prev;

  bool runExpression(char * expr, Zdetector *zd);

  void vapeAlertResponse();
  void tamperAlertResponse();
  void batteryAlertResponse();

  bool vapeAlertActive();
  bool vapeAlertRising();
  bool vapeAlertHigh();
  bool vapeAlertFalling();
  bool vapeAlertLow();
    
  bool tamperAlertActive();
  bool tamperAlertRising();
  bool tamperAlertHigh();
  bool tamperAlertFalling();
  bool tamperAlertLow();
  
  bool batteryAlertActive();
  bool batteryAlertRising();
  bool batteryAlertHigh();
  bool batteryAlertFalling();
  bool batteryAlertLow();

};

extern Zalerts zalerts;

#endif
