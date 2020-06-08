#include <Particle.h>
#include <blynk.h>

#include "zbackhaul.h"

// Adding explicit connect routine that has to work before the rest of the code runs
void Backhaul::connect()
{
  //###### CELLULAR ######
  #if Wiring_Cellular
  bool cellready=Cellular.ready();
  if (!cellready)
  {
    debug("Attempting to connect cellular...\n");
    Cellular.on();
    Cellular.connect();
    waitFor(Cellular.ready,180000);
    if (!Cellular.ready())
    {
    debug("Cellular not ready\n");
    delay(200);
    // timerSleep(3);
    System.reset();
    }
  }
  else
  {
    debug("Cellular ready\n");
  }
  #endif

  //##### WIFI  #####
  #if Wiring_WiFi
  bool cellready = WiFi.ready();
  if (!cellready)
  {
    debug("Attempting to connect WiFi...\n");
    WiFi.on();
    WiFi.connect();
    waitFor(WiFi.ready,180000);
    if (!WiFi.ready())
    {
    debug("WiFi not ready\n");
    delay(200);
    // timerSleep(3);
    System.reset();
    }
  }
  else
  {
    debug("WiFi ready\n");
  }
  #endif  
//*** Commented out for testing without particle connected ***
  bool partconnected = Particle.connected();
  if (!partconnected)
  {
    debug("Attempting to connect to Particle...\n");
    Particle.connect();
    waitFor(Particle.connected,60000);
    if (!Particle.connected())
    {
      debug("Particle not connected\n");
      delay(200);
      // timerSleep(3);
      System.reset();
    } 
  }
  else
  {
    debug("Particle connected\n");
  }
//*** Commented out for testing without particle connected ***
  bool blynkconnected=Blynk.connected();
  if (!blynkconnected)
  {
    debug("Attempting to connect to Blynk...\n");
    #ifdef Version_2
    Blynk.config(auth, "zeptosense2.blynk.cc",portBlynk);
    #else
    Blynk.config(auth, "zeptosense.blynk.cc");
    #endif
    if (!Blynk.connect())
    {
      debug("Blynk not connected\n");
      delay(200);
      // timerSleep(3);
      System.reset();      
    }
  }
  else
  {
    debug("Blynk connected\n");
  }
}

void Backhaul::connectWithoutWaiting()
{
  #if Wiring_Cellular
  bool cellready = Cellular.ready();
  if (!cellready) {
    debug("Cellular not ready\n");
    if (!Cellular.connecting()) {
      debug("Attempting to connect cellular...\n");
      Cellular.on();
      Cellular.connect();
    }
  }
  #endif
  
  #if Wiring_WiFi
  bool wifiready = WiFi.ready();
  if (!wifiready) {
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

bool Backhaul::isConnected()
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

