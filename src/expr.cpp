#include <tinyexpr.h>

#include "zstate.h"
#include "zblynk.h"


bool runExpression(Zstate *st, char *expression)
{   
  double pm1, pm2, pm4, pm10, conc, temp;
  // Store variable names and pointers. 
  te_variable vars[] = {{"pm1", &pm1}, {"pm2", &pm2}, {"pm4", &pm4}, {"pm10", &pm10}, {"conc", &conc}, {"temp", &temp}};
  if (st->terminalDebug) blynk_debug_message(expression); 
  int err;
  // Compile the expression with variables.
  // te_expr *expr = te_compile(_expression.c_str(), vars, 6, &err);
  te_expr *expr = te_compile(expression, vars, 6, &err);

  if (expr) {

      pm1 = (double)Sensor.massPM1;
      pm2 = (double)Sensor.massPM25;
      pm4 = (double)Sensor.massPM4;
      pm10 = (double)Sensor.massPM10;
      conc = (double)conc1;
      temp = (double)temp1;
      const double h2 = te_eval(expr);
      if (st->terminalDebug) blynk_debug_message(datastring);
      if (st->terminalDebug) blynk_debug_message(h2);
      if (st->terminalDebug) blynk_debug_message(InstallLocation);
      if (st->terminalDebug) blynk_debug_message(iSoftwareVersion);

      if (h2==1.0)
      {
        if (st->terminalDebug) blynk_debug_message("The expression is True");
        return true;
      }
      else
      {
        if (st->terminalDebug) blynk_debug_message("The expression is False");
        return false;        
      }     
      te_free(expr);
  } else {
      String mess = "Error near char: " + String(err);
      // debugf("Parse error at %d\n", err);
      if (st->terminalDebug) blynk_debug_message(mess);
      return false;
  } 
}
