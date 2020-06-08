#ifndef ZDETECTOR_H
#define ZDETECTOR_H

class Detector {
 public:
  void setup();
  void clear();
  void read();

  bool tamperIsOn();
  void tamperOff();
  
  float pm1();
  float pm25();
  float pm4();
  float pm10();
  float mass_concentration();
  float numeric_concentration();
  float gas_concentration();
  float tempF();
  float tempC();

 private:
  bool tampered;
  float temperatureF;
  float temperatureC;
  float vgas;
  float gas_conc;
  float pmmass_concentration[4];
  float pmnum_concentration[5];
};

extern Detector detector;

#endif
