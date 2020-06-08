#ifndef ZPMCOUNTER
#define ZPMCOUNTER

class Zpmcounter {
 public:
  void setup();		//do this first
  bool dataAvailable();	//check this, and if ok, then
  void read();		//do this next;

  //after that, query the readings using these:
  float pm1();
  float pm25();
  float pm4();
  float pm10();
  float mass_concentration(int i);
  float numeric_concentration(int i);

private:
  float mass_concentration_data[4];
  float numeric_concentration_data[5];
  void setup_sps30();
  
};

extern Zpmcounter zpmcounter;

#endif
