//###############
//Detection is in 2 parts:
//	1) PM counter
//	2) indoor air quality sensor (IAQ)
//###############


//###############
//PM counter code
//###############
#include <SPS30.h>

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
///////////////////////

  // ULP - Library for reading SPEC Sensors ULP.
  // Created by David E. Peaslee, OCT 27, 2017.
  // Released into the SPEC Sensors domain.


  // The library is built to use several SPEC ULP sensors. An Arduino Uno can handle 3 sensors, since 2 analog pins are required for each sensor.
  // Functions include getTemp(seconds, C/F): returns temperature, setTSpan(seconds, "HIGH"/"LOW"): for calibrating temperature, setVref(R1,R2,R3) for custom sensors,
  // getVgas(seconds): for getting voltage of sensor, getConc(seconds, temperature in degC): for getting temperature corrected concentration,
  // setXSpan(): for setting the calibration factor with known concentration of gas. Variable include _Vcc: the voltage ref of the ADC, _Vsup: the voltage to the ULP, 
  // and _Gain the value of resistor R6. For more details see ULP.cpp in the libraries folder and the data sheet at http://www.spec-sensors.com/wp-content/uploads/2016/06/Analog-SDK-Users-Manual-v18.pdf


// As an example, the ULP is connected this way: Vgas (pin1) to A0, Vtemp (Pin3) to A3, Gnd (Pin6) to Gnd, V+ (Pin7 or Pin8) to 3.3V of Arduino (must not be above 3.3V!!!).

// These constants won't change.  They're used to give names to the pins used and to the sensitivity factors of the sensors:

//const int C1 = A2;
//const int T1 = A4;

const float Sf1 = 4.05; //nA/ppm replace this value with your own sensitivity


float conc1, temp1;
float TZero;
float Vzero1;

IAQ sensor1(A2, A4, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//IAQ sensor1(C1, T1, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//O3 sensor2(C2, T2, Sf2);  //Example O3
//H2S sensor3(C3, T3, Sf3); //Example H2S

//  Include these if using different boards with different voltages
//float ULP::_Vcc = 5.0;  //analogRead Reference Voltage
//float ULP::_Vsup =3.3;  //voltage supplied to V+ of ULP, ideally 3.3 Volts 


void setup_spec()
{
  Sensor.begin();
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

