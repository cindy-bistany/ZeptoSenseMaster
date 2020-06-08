// emacs -*- c++ -*-
//###############
//Detection is in 2 parts:
//	1) PM counter
//	2) indoor air quality sensor (IAQ)
//###############

#include "zdetector.h"
#include "zpmcounter.h"
#include "ziaq.h"
#include "ztamper.h"

Zdetector zdetector;

void Zdetector::setup()
{
  ziaq.setup();
  zpmcounter.setup();
  ztamper.setup();
}

void readings_get()
{
  ztamper.loop();
  zdetector.read();
  zblynk.update_all_readings();

  sensorValid = true;
    String statusMessage;
    String alertMessage;
    // Check to see if there is an alert state and act on it
    if (runExpression(state.expression))     {
      currentAlert = true;
      zblynk.greenAlert();
      zblynk.redAlert();
      //Blynk.virtualWrite(V9,alertGreenImage);
      //Blynk.virtualWrite(V9,alertRedImage);
      
      if (st->timeSynced) statusMessage = "ALRT! "+ Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R") + " " + zdetector.batteryLevel() +"%";
      else statusMessage = "ALERT!             "+ zdetector.batteryLevel() +"%";
      blynk.status_message(statusMessage);
      if (st->terminalDebug) zblynk.debug_message(Time.timeStr() + "|" + String(gmtOffsetSeconds));
    }
    else {
      Blynk.virtualWrite(V0,0);	//what does this do?
      if (st->tamperCurrentAlert){
        st->stateStr="ALRT!";
	zblynk.redAlert();
        //Blynk.virtualWrite(V9,alertRedImage);
      }
      else zblynk_greenAlert();	// Blynk.virtualWrite(V9,alertGreenImage);
      if (timeSynced) statusMessage = st->stateStr + " " + Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R") + " " + zdetector.batteryLevel() + "%";
      else statusMessage = st->stateStr + "                " + field7 + "%";

      int messagesize = statusMessage.length();
      if (messagesize<5){
        statusMessage=String::format("Updating");
        debug(statusMessage + "\n");
        delay(1000);
        System.reset();
      }
        
      Blynk.virtualWrite(V30,statusMessage);
      if (st->terminalDebug) Blynk.virtualWrite(V21, Time.timeStr()+"|"+String(gmtOffsetSeconds));
      currentAlert = false;
    }
     if (VapeBuzzerOn)
     {
         //Turn Buzzer Off if timer expired
         unsigned long vape_elapsed_buzzer =  millis() - VapeAlertBuzzerTime;
  	 alertMessage=String::format("buzzer total time %ul ****", vape_elapsed_buzzer);
         debug(alertMessage + "\n");
         if (vape_elapsed_buzzer>8000){
           digitalWrite(buzzer, LOW);
	         VapeBuzzerOn=false;
	       }
     }

    if (alertChanged()) {
      if (currentAlert) {
          
        if (state.buzzerVapor==true) {
            //Buzzer On
            digitalWrite(buzzer, HIGH);
            VapeAlertBuzzerTime = millis();
            alertMessage=String::format("Vape alert buzzer time reset at %ul ****", VapeAlertBuzzerTime);
            debug(alertMessage + "\n");
	          VapeBuzzerOn=true;
            //delay(8000);
        }
        if (state.notifyVapor==true) {
          #ifdef Version_2
          Blynk.logEvent("vape_alert");
          VapeAlertTime = millis();
          #else
          Blynk.email(state.email, "{DEVICE_NAME} : Alarm", "{DEVICE_NAME} has alarmed!");
          #endif
        }
      }
      else
      {
       unsigned long elapsed = millis() - VapeAlertTime;
  	   alertMessage=String::format("Vape Alert total time %ul milliseconds", elapsed/10);
       debug(alertMessage + "\n");
       if (state.notifyVapor==true)
        {
          #ifdef Version_2
          //Blynk.logEvent("vape_alarm_ended");
          //Particle.publish("Vape Alert",alertMessage,PRIVATE);
          #else
          Blynk.email(state.email, "{DEVICE_NAME} : Alarm ended", "{DEVICE_NAME} alarm ended.");
          #endif
        }
      }
      
    }

    if (batCharge<state.batThreshold) batCurrentAlert = true;
    else batCurrentAlert = false;

    if (batAlertChanged()) {
      if (batCurrentAlert) {
       if (state.notifyBattery==true) {
          #ifdef Version_2
          Blynk.logEvent("low_battery_alert_20");
          #else
          Blynk.email(state.batEmail, "{DEVICE_NAME} : Battery Low Alarm", "{DEVICE_NAME} battery is "+field7+"%!");
          #endif
        }
      }
      else {
       if (state.notifyVapor==true) {
          #ifdef Version_2
          //Blynk.logEvent("low_battery_alert_ended_20");
          //Particle.publish("Low battery","low_battery_alert_ended_20",PRIVATE);
          #else
          Blynk.email(state.batEmail, "{DEVICE_NAME} : Battery Low Alarm ended", "{DEVICE_NAME} battery is "+field7+"%.");
          #endif
        }
      }
      
    }

  }
  else { 
    if (!pmcounter.beginMeasuring()) {
      debug("Unable to read SPS30 - resetting device 1\n");
      delay(1000);
      System.reset();
    }

  }
  if (st->terminalDebug) Blynk.virtualWrite(V21, String(st->readingCount)+"\n");
  debug(millis());
  debug(": ");
  debug(readingCount + "\n");
  debug("\n");
  readingCount++;
}

float Zdetector::read()
{
  const unsigned long timeout = 8100;
  unsigned long startTime = millis();
  
  while ((millis() - startTime) < timeout)
    if (zpmcounter.dataAvailable) {
      zpmcounter.read();
      break;
    }
    else {
      ztamper.loop();
      delay(100);
    }
}

float Zdetector::pm1()  { return zpmcounter.mass_concentration(1); }
float Zdetector::pm25() { return zpmcounter.mass_concentration(1); }
float Zdetector::pm4()  { return zpmcounter.mass_concentration(2); }
float Zdetector::pm10() { return zpmcounter.mass_concentration(3); }

int Zdetector::tempF() { return ziaqsensor.getTemp(1, "F"); }

int Zdetector::tempC() { return ziaqsensor.getTemp(1); }

float Zdetector::mass_concentration(int i) {  return zpmcounter.mass_concentration(i);  }

float Zdetector::numeric_concentration(int i)  {  return zpmcounter.numeric_concentration(i);  }

float Zdetector::vgas()  {  return iaqsensor.getVgas(1);  }

float Zdetector::gas_concentration()  {  return iaqsensor.getConc(1, temperatureC);  }

bool Zdetector::tampered() { return ztamper.tampered(); }
void Zdetector::tamperReset() { ztamper.reset(); }

  
