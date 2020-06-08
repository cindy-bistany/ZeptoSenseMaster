#include <tinyexpr.h>

#include "zdetector.h"

bool runExpression(char *expression, Detector *det)
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
  double calc = te_eval(expr);
  te_free(expr);
  return (calc == 1.0);
}
