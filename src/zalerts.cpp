#include "zalerts.h"

Zakerts zalerts;

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

bool vapeAlert()	{ return vapeTrouble; }
bool batteryAlert()	{ return batteryTrouble; }
bool tamperAlert()	{ return tamperTrouble; }

int Zalerts::update()
{
  vapeTrouble = runExpression(zstate.p.expression, &zdetector);
  batteryTrouble = (zdatector.batteryLevel() < zstate.pp.batteryThreshold);
  tampTrouble = zdetector.tampered();
}
