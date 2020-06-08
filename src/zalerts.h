#ifndef ZALERTS_H
#define ZALERTS_H

class Zalerts {
 public:
  int update();
  
  bool vapeAlert();
  bool batteryAlert();
  bool tamperAlert();
  int error();		//nonzero means error, in expr for example
  
 private:
  bool exprError;
  bool vapeTrouble;
  bool batteryTrouble;
  bool tamperTrouble;

  float runExpression(char * expr, Zdetector *zd);
}

extern Zalerts zalerts;

#endif
