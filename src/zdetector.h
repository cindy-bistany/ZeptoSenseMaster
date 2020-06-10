#ifndef ZDETECTOR_H
#define ZDETECTOR_H

class Zdetector {
 public:
  void setup();
  void clear();
  void read();

  unsigned long lastReadingTime();
  bool tampered();
  void tamperReset();
  
  float pm1();
  float pm25();
  float pm4();
  float pm10();
  float mass_concentration(int i);
  float numeric_concentration(int i);
  float gas_concentration();
  int tempF();
  int tempC();
  int batteryLevel();

 private:
  unsigned long lastReadTime;
};

extern Zdetector zdetector;

#endif
