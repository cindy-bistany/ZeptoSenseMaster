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

  bool isConnected();
  
  void status_message(String msg);
  void debug_message(String msg);


 private:
  bool zblynk_connected;

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

};

extern Zblynk zblynk;

#endif
