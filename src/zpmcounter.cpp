//###############
//PM counter code
//###############
#include <SPS30.h>
#include "zpmcounter.h"
#include "zutil.h"

Zpmcounter zpmcounter;

SPS30 sps30pmcounter;
//From SPS30 we use only:
//	begin()		start the thing
//	dataAvailable()	check if good to read, seems to turn on once and never turn off
//	getMass()	obtain the mass concentration readings, an array of floats
//	getNum()	obtain the numeric concentration, another array of floats


void Zpmcounter::setup_sps30()
{
  // SPS30 checks
  uint32_t sps30start = millis();
  while (true) {
    bool sps30Ok = !sps30pmcounter.begin();
    if (sps30Ok) break;
    if  ((millis()-sps30start) > 8000) crash("Zpmcounter::setup_sps30() - sps30.begin() failed.\n");
    delay(100);
  }
  //must be ok, else we would have crashed

  // Delay 8 seconds or until the SPS30 is ready
  uint32_t waitForSPS30 = millis();
  while (true) {
    if (sps30pmcounter.dataAvailable()) break;
    if ((millis() - waitForSPS30 > 8100)) crash("Zpmcounter::setupsps30) - sps30.dataAvailable() timed out.\n");
    delay(100);
    //check_accel(); WWWWWWW for now skip this during setup, but put back later
  }
}

void Zpmcounter::setup() { setup_sps30(); }
bool Zpmcounter::dataAvailable() { sps30pmcounter.dataAvailable(); }

void Zpmcounter::read()
{
  sps30pmcounter.getMass(mass_concentration_data);
  sps30pmcounter.getNum(numeric_concentration_data);
}

float Zpmcounter::pm1() { return mass_concentration_data[0]; }
float Zpmcounter::pm25() { return mass_concentration_data[1]; }
float Zpmcounter::pm4() { return mass_concentration_data[2]; }
float Zpmcounter::pm10() { return mass_concentration_data[3]; }

float Zpmcounter::mass_concentration(int i) { return mass_concentration_data[i]; }
float Zpmcounter::numeric_concentration(int i) { return numeric_concentration_data[i]; }

