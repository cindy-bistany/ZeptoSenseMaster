#ifndef ZTAMPER
#define ZTAMPER

class Ztamper {
 public:
  void setup();
  void loop();

  bool tampered();
  void reset();

 private:
  bool tamperInterruptTriggered;
  bool tamperInterruptTriggered_prev;

  void rise();
  void stayHigh();
  void fall();
  void stayLow();

  bool isRising();
  bool isHigh();
  bool isFalling();
  bool isLow();

  void setup_adxl345();
  void loop_adxl345();
}

extern Ztamper ztamper;

#endif
