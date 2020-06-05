#ifndef IAQ_H
#define IAQ_H

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
