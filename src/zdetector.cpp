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

Detector detector;

void Detector::setup(Zstate st)
{
  Ziaq.setup();
  Zpmcounter.setup();
}

void readings_get(Zstate *st)
{
  debug("start readSensors\n");
#ifdef KURTDEBUG
  String msg = "";
  msg = msg + "State values are: \n" +
    "Buzzer Tamper " + state.buzzerTamper + "\n" +
    "Buzzer Vapor " + state.buzzerVapor + "\n" +
    "Notify Vapor " + state.notifyVapor + "\n" +
    "Notify Tamper " + state.notifyTamper + "\n" +
    "Notify Battery " + state.notifyBattery + "\n" +
    "Activity Threshold " + state.ActivityThreshold + "\n" +
    "Device Zone " + state.deviceZone + "\n";
  debug(msg);
#endif 
  // Woke up out of a sleep - setup the HW
  if (state.bInSleepMode)
    hw_wakeup();
  st->stateStr = "RDY";
  connectWithoutWaiting();
  setup_rtc();
  
  // Request the GMT offset if Particle is connected and it has not yet been received
  if (Particle.connected&&!gmtOffsetValid)
  {
    publishGMTOffsetRequest();
  }
  checkAccel();
  if (Sensor.dataAvailable()) { 
    Sensor.getMass(mass_concen);
    Sensor.getNum(num_concen);

    debug("--Mass Concentration--\n");
    for(i=0; i<4;i++) {
        debugf("%s: %0.2f\r\n", pm[i+1],mass_concen[i]);
    }
    
    debug("--Number Concentration--\n");
    for(i=0; i<5;i++) {
        debugf("%s: %0.2f\r\n", pm[i],num_concen[i]);
    }
    // Sensor.massPM1, Sensor.massPM25, Sensor.massPM4, Sensor.massPM10     
    field1 = String::format("%0.1f", Sensor.massPM1);
    field2 = String::format("%0.1f", Sensor.massPM25);
    field3 = String::format("%0.1f", Sensor.massPM4);
    field4 = String::format("%0.1f", Sensor.massPM10);

    //////////
    temp1 = sensor1.getTemp(1,"F");  // Use .getTemp(n, "F") to get temp in Fahrenheit, with n as int number of seconds for averaging and "F" or "C" for temp units
    conc1 = sensor1.getConc(1,sensor1.getTemp(1));
    
    debugf("temp1: %f\n", temp1);
    //Use .getVgas(int n) where n is the number of seconds to average
    //Use ._Vref to read the reference voltage (voltage offset)
    debugf("Vgas: %f\n", sensor1.getVgas(1));
    //Use .getConc(1, temp1) where temp1 is in deg C for temperature corrected span
    debugf("Conc: %f\n", conc1);

    field5 = String::format("%0.1f", conc1);
    field6 = String::format("%0.1f", temp1);

    /////////

    #if Wiring_Cellular
    float batCharge = fuel.getSoC();
    field7 = String::format("%0.0f", batCharge);
    CellularSignal sig = Cellular.RSSI();
    field8 = String::format("%d", 100*sig.qual/49);
    #endif

    #if Wiring_WiFi
    float voltage = analogRead(BATT) * 0.0011224;
    float batCharge = exp(5.0601*voltage)*0.0000001;
    if (batCharge>100) batCharge = 100;
    field7 = String::format("%0.0f", batCharge);
    WiFiSignal sig = WiFi.RSSI();
    field8 = String::format("%d", (int)sig.getQuality());
    #endif

    datastring = field1+":"+field2+":"+field3+":"+field4+":"+field5+":"+field6+":"+field7+":"+field8;

    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    // Blynk.virtualWrite(V1, Sensor.massPM1);
    // Blynk.virtualWrite(V2, Sensor.massPM25);
    // Blynk.virtualWrite(V3, Sensor.massPM4);
    // Blynk.virtualWrite(V4, Sensor.massPM10);
    // Blynk.virtualWrite(V5, conc);
    // Blynk.virtualWrite(V6, temp1);
    // Blynk.virtualWrite(V7, fuel.getSoC());
    // Blynk.virtualWrite(V8, sig.qual);
    Blynk.virtualWrite(V1, field1);
    Blynk.virtualWrite(V2, field2);
    Blynk.virtualWrite(V3, field3);
    Blynk.virtualWrite(V4, field4);
    Blynk.virtualWrite(V5, field5);
    Blynk.virtualWrite(V6, field6);
    Blynk.virtualWrite(V7, field7);
    Blynk.virtualWrite(V8, field8);   

    // Particle.publish("Test readings",datastring,60,PRIVATE);
    debug(datastring + "\n");
    sensorValid = true;
    String statusMessage;
    String alertMessage;
    // Check to see if there is an alert state and act on it
    if (runExpression(state.expression))
    {
      currentAlert = true;
      Blynk.virtualWrite(V9,alertGreenImage);
      Blynk.virtualWrite(V9,alertRedImage);
      
      if (st->timeSynced)
      {
        // statusMessage = "ALERT! "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%D %R")+" "+field7+"%";
        statusMessage = "ALRT! "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R")+" "+field7+"%";
      }
      else
      {
        statusMessage = "ALERT!             "+field7+"%";
      }
      Blynk.virtualWrite(V30,statusMessage);
      if (st->terminalDebug) Blynk.virtualWrite(V21, Time.timeStr()+"|"+String(gmtOffsetSeconds));
    }
    else
    {
      Blynk.virtualWrite(V0,0);
      if (st->tamperCurrentAlert){
        st->stateStr="ALRT!";
        Blynk.virtualWrite(V9,alertRedImage);
      }
      else
        Blynk.virtualWrite(V9,alertGreenImage);
      if (timeSynced)
      {
        // statusMessage = "OK "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%D %R")+" "+field7+"%";
        statusMessage = st->stateStr+" "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R")+" "+field7+"%";
      }
      else
      {
        statusMessage = st->stateStr + "                " + field7 + "%";
      }
      int messagesize=statusMessage.length();
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

float Detector::read()
{
  const unsigned long timeout = 8100;
  unsigned long startTime = millis();
  
  while ((millis() - startTime) < timeout)
    if (zpmcounter.dataAvailable) {
      zpmcounter.read();
      break;
    }
    else {
      zstate.tampered |= ztamper.tampered();
      delay(100);
    }
}

float Detector::pm1()  { return zpmcounter.mass_concentration(1); }
float Detector::pm25() { return zpmcounter.mass_concentration(1); }
float Detector::pm4()  { return zpmcounter.mass_concentration(2); }
float Detector::pm10() { return zpmcounter.mass_concentration(3); }

int tempF() { return ziaqsensor.getTemp(1, "F"); }

int tempC() { return ziaqsensor.getTemp(1); }

float Detector::mass_concentration(int i) {  return zpmcounter.mass_concentration(i);  }

float Detector::numeric_concentration(int i)  {  return zpmcounter.numeric_concentration(i);  }

float Detector::vgas()  {  return iaqsensor.getVgas(1);  }

float Detector::gas_concentration()  {  return iaqsensor.getConc(1, temperatureC);  }


  
