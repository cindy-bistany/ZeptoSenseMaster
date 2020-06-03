/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/detection.ino"
//###############
//Detection is in 2 parts:
//	1) PM counter
//	2) indoor air quality sensor (IAQ)
//###############


//###############
//PM counter code
//###############
#include <SPS30.h>

void setup_sps30(State st);
void loop_sps30(State st, Readings rd);
void loop_pmcounter(State st, Readings rd);
void setup_spec();
void loop_spec();
void setup_iaq(Zstate st);
void loop_iaq(Zstate st);
void setup_detection(Zstate st);
void loop_detection(zstate *st);
#line 13 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/detection.ino"
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


//###############
//SPEC Sensors code
//###############
#include "ULP.h"

IoTNodePower power;
FuelGauge fuel;
IAQ sensor1(A2, A4, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//IAQ sensor1(C1, T1, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//O3 sensor2(C2, T2, Sf2);  //Example O3
//H2S sensor3(C3, T3, Sf3); //Example H2S

// These constants won't change.  They're used to give names to the pins used and to the sensitivity factors of the sensors:

//const int C1 = A2;
//const int T1 = A4;

const float Sf1 = 4.05; //nA/ppm replace this value with your own sensitivity


void setup_spec()
{
  Sensor.begin();
}

void loop_spec()
{
  float conc1, temp1;
  float TZero;
  float Vzero1;
  
  //  Include these if using different boards with different voltages
  //float ULP::_Vcc = 5.0;  //analogRead Reference Voltage
  //float ULP::_Vsup =3.3;  //voltage supplied to V+ of ULP, ideally 3.3 Volts 
  
  String field1,field2,field3,field4,field5,field6,field7,field8;
  String datastring;

}

void setup_iaq(Zstate st) { setup_spec(st); }

void loop_iaq(Zstate st) { loop_spec(st); }

void setup_detection(Zstate st)
{
  setup_iaq(st);
  setup_pmcounter();
}

void loop_detection(zstate *st)
{
  loop_iaq(st);
  loop_pmcounter(st);
}

