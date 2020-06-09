#ifndef ZBLYNK_H
#define ZBLYNK_H

#include "application.h"

#include <blynk.h>
#include "zstate.h"

class Zblynk {
 public:
  void setup();
  void update();
  void update_all();
  void update_all_state();
  void update_all_readings();
  
  bool isConnected();
  
  void redAlert();
  void greenAlert();
  void status_message(String msg);
  void debug_message(String msg);


 private:
  bool zblynk_connected;

  //configuration
  void write_numberOfReadings(int numOfReadings);
  void write_secondsBetweenReadings(int secBetwReadings);
  void write_zeroOffset(float zo);
  void write_expression(char *expr);
  void write_batteryThreshold(float thr);
  void write_deviceTimeZone(int tz);
  void write_enableVapeBuzzer(bool vb);
  void write_enableTamperBuzzer(bool tb);
  void write_enableVapeNotify(bool vn);
  void write_enableBatteryNotify(bool bn);
  void write_enableTamperNotify(bool tn);
  void write_vapeEmail(char *email);
  void write_batteryEmail(char *email);
  void write_tamperEmail(char *email);
  void write_enterCode(char *msg);
  void write_activityThreshold(int at);

  //Readings
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
