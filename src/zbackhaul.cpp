#include <Particle.h>

#include "zbaseboard.h"
#include "zbackhaul.h"
#include "zblynk.h"

#define BACKHAUL_IS_WIFI true
#define BACKHAUL_IS_CELL false

Zbackhaul zbackhaul;

void Zbackhaul::setup() { connect(true); }

int Zbackhaul::signalStrength()
{
#if BACKHAUL_IS_CELL
    CellularSignal sig = Cellular.RSSI();
    return int(100*sig.qual/49);
#endif

#if BACKHAUL_IS_WIFI
    WiFiSignal sig = WiFi.RSSI();
    return int(sig.getQuality());
#endif
}

bool Zbackhaul::isWifi()     { return BACKHAUL_IS_WIFI;  }
bool Zbackhaul::isCellular() { return BACKHAUL_IS_CELL; }

// Adding explicit connect routine that has to work before the rest of the code runs
void Zbackhaul::connect(bool blocking)
{
#if BACKHAUL_IS_CELL
  if (!Cellular.ready()) {
    debug("Attempting to connect cellular...\n");
    Cellular.on();
    Cellular.connect();
    if (blocking) {
      waitFor(Cellular.ready, 180000);
      if (!Cellular.ready()) crash("Cellular not ready\n");
    }
  }
  debug("Cellular is ready\n");
#endif

#if BACKHAUL_IS_WIFI
  if (!WiFi.ready()) {
    WiFi.on();
    WiFi.connect();
    if (blocking) {
      waitFor(WiFi.ready,180000);
      if (!WiFi.ready()) crash(HORN_CRASH_NONET);
    }
  }
  if (WiFi.ready()) zbaseboard.morse("ok wifi");
#endif  

  if (!zblynk.isConnected()) {
    debug("Attempting to connect to Blynk...\n");
    zblynk.setup();
  }
  if (zblynk.isConnected()) zbaseboard.morse("ok blyn");

  if (!Particle.connected()) {
    Particle.connect();
    if (blocking) {
      waitFor(Particle.connected, 60000);
      if (!Particle.connected()) crash(HORN_CRASH_NOPARTICLE);
    }
  }
  if (Particle.connected()) zbaseboard.morse("ok part");
  debug("Particle connected\n");

}

bool Zbackhaul::isConnected()
{
  bool connected = true;
#if BACKHAUL_IS_CELL
  if (!Cellular.ready()) {
    debug("Cellular not ready\n");
    connected = false;
  }
#endif

#if BACKHAUL_IS_WIFI
  if (!WiFi.ready()) {
    debug("WiFi not ready\n");
    connected = false;
  }
#endif
  
  if (!Particle.connected) {
    debug("Particle not connected\n");
    connected = false;
  }
  
  if (!zblynk.isConnected()) {
    debug("Blynk not connected\n");
    connected = false;
  }
  
  return connected;
}

void Zbackhaul::shutdown()
{
  
#if BACKHAUL_IS_CELL
  Cellular.off();
#endif

#if BACKHAUL_IS_WIFI
  WiFi.off();
#endif
  
}

FuelGauge fuel;

float Zbackhaul::batteryLevel()
{
  if (zbackhaul.isCellular()) return fuel.getSoC();
  else if (zbackhaul.isWifi()) {
    float voltage = analogRead(BATT) * 0.0011224;
    float batCharge = exp(5.0601*voltage)*0.0000001;
    if (batCharge>100) batCharge = 100;
    return batCharge;
  }
  else crash("Zbackhaul::batteryLevel() - logic error.");
  return 0;
}
