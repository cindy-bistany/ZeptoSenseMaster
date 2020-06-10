// emacs -*- c++ -*-
//###############
//Detection is in 2 parts:
//	1) PM counter
//	2) indoor air quality sensor (IAQ)
//###############

#include "zdetector.h"
#include "zpmcounter.h"
#include "ziaq.h"
#include "ztamper.h"
#include "zblynk.h"
#include "zbaseboard.h"

Zdetector zdetector;

void Zdetector::setup()
{
  ziaq.setup();
  zpmcounter.setup();
  ztamper.setup();
  read();
}


void Zdetector::read()
{
  const unsigned long timeout = 8100;
  unsigned long startTime = millis();
  
  while ((millis() - startTime) < timeout)
    if (zpmcounter.dataAvailable()) {
      zpmcounter.read();
      break;
    }
    else {
      ztamper.loop();
      delay(100);
    }
  lastReadTime = millis();
}

unsigned long Zdetector::lastReadingTime() { return lastReadTime; }

float Zdetector::pm1()  { return zpmcounter.mass_concentration(0); }
float Zdetector::pm25() { return zpmcounter.mass_concentration(1); }
float Zdetector::pm4()  { return zpmcounter.mass_concentration(2); }
float Zdetector::pm10() { return zpmcounter.mass_concentration(3); }

float Zdetector::gas_concentration()	{  return ziaq.gas_concentration();  }
int Zdetector::tempF()			{  return ziaq.tempF(); }
int Zdetector::tempC()			{  return ziaq.tempC(); }

float Zdetector::mass_concentration(int i)	{  return zpmcounter.mass_concentration(i);  }
float Zdetector::numeric_concentration(int i)	{  return zpmcounter.numeric_concentration(i);  }

int Zdetector::batteryLevel()	{  return zbaseboard.batteryLevel();  }
bool Zdetector::tampered()	{  return ztamper.tampered(); }
void Zdetector::tamperReset()	{  ztamper.reset(); }

  
