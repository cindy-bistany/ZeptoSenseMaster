#ifndef ZBLYNK_H
#define ZBLYNK_H

#include "application.h"

#include "zstate.h"

#define ZBLYNKPORT 8080

class Zblynk {
 public:
  void setup();
  void update();
  void update_all();
  void update_all_state();
  void update_all_readings();
  
  bool isConnected();
  void logEvent(char *s);
  void config(char *auth, char *url, int port);
  
  void redAlert();
  void greenAlert();
  void status_message(String msg);
  void debug_message(String msg);

  //This is managed privately, but access from external callback is required, so public here.
  //So don't touch this or look at it from outside the class methods.
  //Use isConnected() instead.
  //Likewise, these are not useful outside the class, but require access from blynk callback.
  void write_enterCode();
  void write_zeroOffset();

 private:

  //Write configuration to blynk
  void write_numberOfReadings();
  void write_secondsBetweenReadings();  
  void write_expression();
  void write_batteryThreshold();
  void write_deviceTimeZone();
  void write_enableVapeBuzzer();
  void write_enableTamperBuzzer();
  void write_enableVapeNotify();
  void write_enableBatteryNotify();
  void write_enableTamperNotify();
  void write_vapeEmail();
  void write_batteryEmail();
  void write_tamperEmail();
  void write_activityThreshold();
  
  //Write readings to blynk
  void write_pm1();
  void write_pm25();
  void write_pm4();
  void write_pm10();
  void write_gas_concentration();
  void write_tempF();
  void write_batteryLevel();
  void write_signalStrength();

};

extern Zblynk zblynk;

#endif
