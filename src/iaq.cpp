
//###############
//SPEC Sensors code
//###############
#include <ULP.h>
#include "ziaq.h"

const float Sf1 = 4.05; //nA/ppm replace this value with your own sensitivity
IAQ iaqsensor(A2, A4, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)

//IAQ sensor1(C1, T1, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//O3 sensor2(C2, T2, Sf2);  //Example O3
//H2S sensor3(C3, T3, Sf3); //Example H2S

// These constants won't change.  They're used to give names to the pins used and to the sensitivity factors of the sensors:
//const int C1 = A2;
//const int T1 = A4;

void Ziaq::setup() { iaqsensor.begin(); }
void Ziaq::tempF() { iaqsensor.getTemp(1, "F"); }
void Ziaq::tempC() { iaqsensor.getTemp(1, "C"); }
void Ziaq::vgas()  { iaqsensor.getVgas(1); }


void Ziaq::concentration()
{
  iaqsensor.getConc(1, iaqsensor.getTemp(1, "C"));
}
