/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/detection.ino"
void setup_sps30(State st);
void loop_sps30(State st, Readings rd);
void loop_pmcounter(State st, Readings rd);
void setup_spec();
void setup_iaq(State st, Readings rd);
void loop_spec();
void loop_iaq(State, st, Readings rd);
void setup_detection(State st, Readings rd);
void loop_detection(State st , Readings rd);
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/detection.ino"
SPS30 sps30;

void setup_sps30(State st)
{
  // SPS30 checks
  bool sps30OK = false;
  uint32_t sps30start = millis();
  
  while (!sps30OK && millis()-sps30start<8000) {
    if (!sps30.begin()) {
      debug("Unable to read SPS30 - trying again\n");
      delay(1000);
    }
    else {
      debug("Read SPS30\n");
      sps30OK = true;
    }
  }
    
  if (!sps30OK) {
    debug("Unable to read SPS30 - resetting device\n");
    delay(1000);
    System.reset();    
  }

  // Delay 8 seconds or until the SPS30 is ready
  unsigned long waitForSPS30 = millis();
  while ((!sps30.dataAvailable()) && (millis()-waitForSPS30<8100)) {
    delay(100);
    check_accel();
  }
}

void loop_sps30(State st, Readings rd)
{
}

void loop_pmcounter(State st, Readings rd)
{
  loop_sps30(st, rd);
}

void setup_spec()
{

}
void setup_iaq(State st, Readings rd) { setup_spec(st, rd); }

void loop_spec()
{
}
void loop_iaq(State, st, Readings rd) { loop_spec(st, rd); }

void setup_detection(State st, Readings rd)
{
  setup_iaq();
  setup_pmcounter();
}

void loop_detection(State st , Readings rd)
{
  loop_iaq(st, rd);
  loop_pmcounter(st, rd);
}

