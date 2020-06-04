/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/zblynk.ino"
// emacs -*- c++ -*-
//zblynk - glue code between blynk and zeptive

void blynk_status_message(String msg);
#line 4 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/zblynk.ino"
void blynk_status_message(String msg)
{
  Blynk.virtualWrite(V30, msg);
}
