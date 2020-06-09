#ifndef ZRESPONSE_H
#define ZRESPONSE_H

class Zresponse {
 public:
  void setup();
  void loop();
  void respond();
  

 private:
  bool vapeAlert_prev;
  bool tamperAlert_prev;
  bool batteryAlert_prev;

  void vapeResponse();
  void tamperResponse();
  void batteryResponse();

  bool vapeRising();
  bool vapeHigh();
  bool vapeFalling();
  bool vapeLow();
    
  bool tamperRising();
  bool tamperHigh();
  bool tamperFalling();
  bool tamperLow();
    
  bool batteryRising();
  bool batteryHigh();
  bool batteryFalling();
  bool batteryLow();
};

extern Zresponse zresponse;

#endif
