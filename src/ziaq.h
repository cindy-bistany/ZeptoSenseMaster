#ifndef ZIAQ_H
#define ZIAQ_H

class Ziaq {
 public:
  void setup();
  int tempF();
  int tempC();
  float gas_concentration();
  float zero();
  
 private:
  float vgas();
};

extern Ziaq ziaq;

#endif
