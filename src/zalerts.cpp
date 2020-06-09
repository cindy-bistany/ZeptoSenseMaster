#include <tinyexpr.h>

#include "zalerts.h"
#include "zstate.h"
#include "zbaseboard.h"
#include "zblynk.h"
#include "zdetector.h"
#include "zclock.h"

Zalerts zalerts;

void Zalerts::clear() {  vapeTrouble = batteryTrouble = tamperTrouble = false;  }
void Zalerts::setup() { clear(); }

bool Zalerts::runExpression(char *expression, Zdetector *det)
{   
  // Setup variable list for tinyexpr
  double pm1 = (double) det->pm1();
  double pm2 = (double) det->pm25();
  double pm4 = (double) det->pm4();
  double pm10 = (double) det->pm10();
  double conc = (double) det->gas_concentration();
  double temp = (double) det->tempF();
  te_variable vars[] = {{"pm1", &pm1}, {"pm2", &pm2}, {"pm4", &pm4}, {"pm10", &pm10}, {"conc", &conc}, {"temp", &temp}};
  int err = 0;

  te_expr *expr = te_compile(expression, vars, 6, &err);
  if (!expr) return false;
  if (err) return false;
  double calc = te_eval(expr);
  te_free(expr);
  return (calc == 1.0);
}

int Zalerts::update()
{
  vapeTrouble_prev = vapeTrouble;
  vapeTrouble = runExpression(zstate.p.expression, &zdetector);

  batteryTrouble_prev = batteryTrouble;
  batteryTrouble = (zbaseboard.batteryLevel() < zstate.p.batteryThreshold);

  tamperTrouble_prev = tamperTrouble;
  tamperTrouble = zdetector.tampered();

  vapeAlertResponse();
  batteryAlertResponse();
  tamperAlertResponse();
}

void Zalerts::vapeAlertResponse()
{
  static long vapeAlertStartTime = 0;
  static long vapeBuzzerStartTime = 0;
  static bool vapeBuzzerIsOn = false;
  
  zblynk.greenAlert();
  zblynk.redAlert();
  if (vapeAlertRising()) {
    vapeAlertStartTime = millis();
    
    if (zstate.p.enableNotifyVape) {
      String statusMessage = "ALRT! "+ Time.format(zclock.now(),"%h%e %R") + " " + zdetector.batteryLevel() + "%";
      zblynk.status_message(statusMessage);
      Blynk.logEvent("vape_alert");
    }
    
    if (zstate.p.enableBuzzerVape) {
      zbaseboard.buzzer(true);
      vapeBuzzerIsOn = true;
      vapeBuzzerStartTime = millis();
    }
    return;
  }

  if (!vapeAlertActive()) {
    if (vapeBuzzerIsOn) {
      long vapeBuzzerActiveTime = millis() - vapeBuzzerStartTime;
      if (vapeBuzzerActiveTime > 8000); {
	zbaseboard.buzzer(false);
	vapeBuzzerIsOn = false;
      }
    }
    return;
  }
}

void Zalerts::tamperAlertResponse()
{
  static long tamperBuzzerStartTime = 0;
  static bool tamperBuzzerIsOn = false;
  
  zblynk.greenAlert();
  zblynk.redAlert();
  if (tamperAlertRising()) {

    if (zstate.p.enableNotifyTamper) {
      String statusMessage = "ALRT! "+ Time.format(zclock.now(),"%h%e %R") + " " + zdetector.batteryLevel() + "%";
      zblynk.status_message(statusMessage);
      Blynk.logEvent("tamper_alert");
    }
    
    if (zstate.p.enableBuzzerTamper) {
      zbaseboard.buzzer(true);
      tamperBuzzerIsOn = true;
      tamperBuzzerStartTime = millis();
      zbaseboard.buzzer(false);
    }
    return;
  }
  
  if (!tamperAlertActive()) {
    if (tamperBuzzerIsOn) {
      long tamperBuzzerActiveTime = millis() - tamperBuzzerStartTime;
      if (tamperBuzzerActiveTime > 8000); {
	zbaseboard.buzzer(false);
	tamperBuzzerIsOn = false;
      }
    }
    return;
  }
  
}

void Zalerts::batteryAlertResponse()
{
  static long batteryBuzzerStartTime = 0;
  static long batteryBuzzerStopTime = 0;
  static bool batteryBuzzerIsOn = false;
  
  if (batteryAlertActive()) {

    zblynk.greenAlert();
    zblynk.redAlert();

    if (zstate.p.enableNotifyBattery) {
      String statusMessage = "ALRT! "+ Time.format(zclock.now(),"%h%e %R") + " " + zdetector.batteryLevel() + "%";
      zblynk.status_message(statusMessage);
      Blynk.logEvent("battery_alert");
    }
    
    if (zstate.p.enableBuzzerBattery) {
      long elapsed = millis() - batteryBuzzerStartTime;
      if (elapsed > 60*60*1000100) {
	zbaseboard.buzzer(false);
	zbaseboard.beep("...---...");
	batteryBuzzerStartTime = millis();
	zbaseboard.buzzer(false);
      }
    }
  }
}

bool Zalerts::vapeAlertActive()		{  return ( vapeTrouble ||  vapeTrouble_prev); }
bool Zalerts::vapeAlertRising()		{  return ( vapeTrouble && !vapeTrouble_prev); }
bool Zalerts::vapeAlertHigh()		{  return ( vapeTrouble &&  vapeTrouble_prev); }
bool Zalerts::vapeAlertFalling()       	{  return (!vapeTrouble &&  vapeTrouble_prev); }
bool Zalerts::vapeAlertLow()		{  return (!vapeTrouble && !vapeTrouble_prev); }
    
bool Zalerts::tamperAlertActive()	{  return ( tamperTrouble ||  tamperTrouble_prev); }
bool Zalerts::tamperAlertRising()	{  return ( tamperTrouble && !tamperTrouble_prev); }
bool Zalerts::tamperAlertHigh()		{  return ( tamperTrouble &&  tamperTrouble_prev); }
bool Zalerts::tamperAlertFalling()	{  return (!tamperTrouble &&  tamperTrouble_prev); }
bool Zalerts::tamperAlertLow()		{  return (!tamperTrouble && !tamperTrouble_prev); }
    
bool Zalerts::batteryAlertActive()	{  return ( batteryTrouble ||  batteryTrouble_prev); }
bool Zalerts::batteryAlertRising()	{  return ( batteryTrouble && !batteryTrouble_prev); }
bool Zalerts::batteryAlertHigh()       	{  return ( batteryTrouble &&  batteryTrouble_prev); }
bool Zalerts::batteryAlertFalling()	{  return (!batteryTrouble &&  batteryTrouble_prev); }
bool Zalerts::batteryAlertLow()		{  return (!batteryTrouble && !batteryTrouble_prev); }
