//###############
//PM counter code
//###############
#include <SPS30.h>

SPS30 pmcounter;

void Detector::setup_sps30(State st)
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

void Detector::setup_pmcounter() { setup_sps30(); }

