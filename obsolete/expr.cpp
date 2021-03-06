/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/expr.ino"
#include "tinyexpr.h"

bool runExpression(Zstate *st, char *expression);
#line 3 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/expr.ino"
bool runExpression(Zstate *st, char *expression)
{   
  double pm1, pm2, pm4, pm10, conc, temp;
  // Store variable names and pointers. 
  te_variable vars[] = {{"pm1", &pm1}, {"pm2", &pm2}, {"pm4", &pm4}, {"pm10", &pm10}, {"conc", &conc}, {"temp", &temp}};
  if (st->terminalDebug) Blynk.virtualWrite(V21, expression); 
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
      if (terminalDebug) Blynk.virtualWrite(V21, datastring);
      if (terminalDebug) Blynk.virtualWrite(V21, h2);
      if (terminalDebug) Blynk.virtualWrite(V21, InstallLocation);
      if (terminalDebug) Blynk.virtualWrite(V21, iSoftwareVersion);

      if (h2==1.0)
      {
        if (terminalDebug) Blynk.virtualWrite(V21, "The expression is True");
        return true;
      }
      else
      {
        if (terminalDebug) Blynk.virtualWrite(V21, "The expression is False");
        return false;        
      }     
      te_free(expr);
  } else {
      String mess = "Error near char: " + String(err);
      // debugf("Parse error at %d\n", err);
      if (terminalDebug) Blynk.virtualWrite(V21, mess);
      return false;
  } 
}
