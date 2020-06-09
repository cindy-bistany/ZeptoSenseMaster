#include "zresponse.h"
#include "zalerts.h"


void Zresponse::vapeResponse()
{
  zblynk.greenAlert();
  zblynk.redAlert();
  if (vapeRising()) {
    String statusMessage = "ALRT! "+ Time.format(rtc.rtcNow() + gmtOffsetSeconds,"%h%e %R") + " " + zdetector.batteryLevel() + "%";
    blynk.status_message(statusMessage);
  }
}

void Zresponse::tamperResponse()
{
}

void Zresponse::batteryResponse(){}

bool Zresponse::vapeRising()		{  return (zalerts.vapeAlert() && vapeAlert_prev); }
bool Zresponse::vapeHigh()		{}
bool Zresponse::vapeFalling()		{}
bool Zresponse::vapeLow()		{}
    
bool Zresponse::tamperRising()		{}
bool Zresponse::tamperHigh()		{}
bool Zresponse::tamperFalling()		{}
bool Zresponse::tamperLow()		{}
    
bool Zresponse::batteryRising()		{}
bool Zresponse::batteryHigh()		{}
bool Zresponse::batteryFalling()	{}
bool Zresponse::batteryLow()		{}
