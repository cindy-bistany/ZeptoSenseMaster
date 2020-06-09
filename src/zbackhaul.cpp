#include <Particle.h>
#include <blynk.h>

#include "zbackhaul.h"

void Zbackhaul::setup() { connect(); }

// Adding explicit connect routine that has to work before the rest of the code runs
void Zbackhaul::connect()
{
  bool networkReady = false;
  
  //###### CELLULAR ######
  #if Wiring_Cellular
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
  #if Wiring_WiFi
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

  if (!Blynk.connected()) {
    debug("Attempting to connect to Blynk...\n");
    #ifdef Version_2
    Blynk.config(auth, "zeptosense2.blynk.cc",portBlynk);
    #else
    Blynk.config(auth, "zeptosense.blynk.cc");
    #endif
    if (!Blynk.connected()) crash("Blynk not connected\n");
  }
  debug("Blynk connected\n");
}

void Zbackhaul::connectWithoutWaiting()
{
  bool networkReady = false;
  
  #if Wiring_Cellular
  if (!Cellular.ready) {
    debug("Cellular not ready\n");
    if (!Cellular.connecting()) {
      debug("Attempting to connect cellular...\n");
      Cellular.on();
      Cellular.connect();
    }
  }
  #endif
  
  #if Wiring_WiFi
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
    if (!Blynk.connected()) {
      debug("Attempting to connect to Blynk...\n");
      Blynk.run();
      if (!Blynk.connect())
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
#if Wiring_Cellular
  if (!Cellular.ready()) {
    debug("Cellular not ready\n");
    connected = false;
  }
#endif

#if Wiring_WiFi
  if (!WiFi.ready()) {
    debug("WiFi not ready\n");
    connected = false;
  }
#endif
  
  if (!Particle.connected) {
    debug("Particle not connected\n");
    connected = false;
  }
  
  if (!Blynk.connected()) {
    debug("Blynk not connected\n");
    connected = false;
  }
  
  return connected;
}

