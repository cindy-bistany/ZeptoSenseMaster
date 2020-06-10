#include <Particle.h>

#include "zbackhaul.h"
#include "zblynk.h"

#define BACKHAUL_IS_WIFI true
#define BACKHAUL_IS_CELL false

Zbackhaul zbackhaul;

void Zbackhaul::setup() { connect(); }

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

bool Zbackhaul::isWifi()  {  return BACKHAUL_IS_WIFI;  }
bool Zbackhaul::isCellular() { return BACKHAUL_IS_WIFI; }

// Adding explicit connect routine that has to work before the rest of the code runs
void Zbackhaul::connect()
{
  bool networkReady = false;
  
  //###### CELLULAR ######
#if BACKHAUL_IS_CELL
  if (!Cellular.ready()) {
    debug("Attempting to connect cellular...\n");
    Cellular.on();
    Cellular.connect();
    waitFor(Cellular.ready, 180000);
    if (!Cellular.ready()) crash("Cellular not ready\n");
  }
  networkReady = Cellular.ready();
  debug("Cellular is ready\n");
#endif

  //##### WIFI  #####
#if BACKHAUL_IS_WIFI
  if (!WiFi.ready()) {
    debug("Attempting to connect WiFi...\n");
    WiFi.on();
    WiFi.connect();
    waitFor(WiFi.ready,180000);
    if (!WiFi.ready()) crash("WiFi not ready\n");
  }
  networkReady = WiFi.ready();
  debug("WiFi ready\n");
#endif  

  if (!networkReady) crash("No network available.");
  
  if (!Particle.connected()) {
    debug("Attempting to connect to Particle...\n");
    Particle.connect();
    waitFor(Particle.connected,60000);
    if (!Particle.connected()) crash("Particle not connected\n");
  }
  debug("Particle connected\n");

  if (!zblynk.isConnected()) {
    debug("Attempting to connect to Blynk...\n");
    zblynk.config(auth, "zeptosense2.blynk.cc", ZBLYNKPORT);
    if (!zblynk.isConnected()) crash("Blynk not connected\n");
  }
  debug("Blynk connected\n");
}

void Zbackhaul::connectWithoutWaiting()
{
  bool networkReady = false;
  
  #if BACKHAUL_IS_CELL
  if (!Cellular.ready) {
    debug("Cellular not ready\n");
    if (!Cellular.connecting()) {
      debug("Attempting to connect cellular...\n");
      Cellular.on();
      Cellular.connect();
    }
  }
  #endif
  
  #if BACKHAUL_IS_WIFI
  if (!WiFi.ready()) {
    debug("WiFi not ready\n");
    if (!WiFi.connecting()) {
      debug("Attempting to connect WiFi...\n");
      WiFi.on();
      WiFi.connect();
    }
  }
  #endif
  else {
    if (!zblynk.isConnected()) {
      debug("Attempting to connect to Blynk...\n");
      zblynk.setup();
      if (!zblynk.isConnected())
        debug("Blynk not connected\n");
    }
    else debug("Blynk connected\n");
    
  //*** Commented out for testing without particle connected ***
    if (!Particle.connected()) {
      debug("Attempting to connect to Particle...\n");
      Particle.connect();
      if (!Particle.connected())
        debug("Particle not connected\n");
    }
    else debug("Particle connected\n");
  }
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
}
