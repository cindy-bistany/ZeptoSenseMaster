#ifndef ZIAQ_H
#define ZIAQ_H

class Ziaq {
 public:
  void setup();
  float concentration();

 private:
  int tempF();
  int tempC();
  float vgas();
};

extern Ziaq ziaq;

#endif
