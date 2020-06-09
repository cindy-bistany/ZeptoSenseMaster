#ifndef ZIAQ_H
#define ZIAQ_H

class Ziaq {
 public:
  void setup();
  int tempF();
  int tempC();
  float gas_concentration();

 private:
  float vgas();
};

extern Ziaq ziaq;

#endif
