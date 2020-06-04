// emacs -*- c++ -*-
//###############
//Detection is in 2 parts:
//	1) PM counter
//	2) indoor air quality sensor (IAQ)
//###############


//###############
//PM counter code
//###############
#include <SPS30.h>

SPS30 sps30;

void setup_sps30(State st)
{
  // SPS30 checks
  bool sps30OK = false;
  uint32_t sps30start = millis();
  
  while (!sps30OK && millis()-sps30start<8000) {
    if (!sps30.begin()) {
      debug("Unable to read SPS30 - trying again\n");
      delay(1000);
    }
    else {
      debug("Read SPS30\n");
      sps30OK = true;
    }
  }
    
  if (!sps30OK) {
    debug("Unable to read SPS30 - resetting device\n");
    delay(1000);
    System.reset();    
  }

  // Delay 8 seconds or until the SPS30 is ready
  unsigned long waitForSPS30 = millis();
  while ((!sps30.dataAvailable()) && (millis()-waitForSPS30<8100)) {
    delay(100);
    check_accel();
  }
}

void loop_sps30(State st, Readings rd)
{
}

void loop_pmcounter(State st, Readings rd)
{
  loop_sps30(st, rd);
}


//###############
//SPEC Sensors code
//###############
#include "ULP.h"

IAQ sensor1(A2, A4, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//IAQ sensor1(C1, T1, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//O3 sensor2(C2, T2, Sf2);  //Example O3
//H2S sensor3(C3, T3, Sf3); //Example H2S

// These constants won't change.  They're used to give names to the pins used and to the sensitivity factors of the sensors:

//const int C1 = A2;
//const int T1 = A4;

const float Sf1 = 4.05; //nA/ppm replace this value with your own sensitivity


void setup_spec()
{
  Sensor.begin();
}

void loop_spec()
{
  float conc1, temp1;
  float TZero;
  float Vzero1;
  
  //  Include these if using different boards with different voltages
  //float ULP::_Vcc = 5.0;  //analogRead Reference Voltage
  //float ULP::_Vsup =3.3;  //voltage supplied to V+ of ULP, ideally 3.3 Volts 
  
  String field1,field2,field3,field4,field5,field6,field7,field8;
  String datastring;

}

void setup_iaq(Zstate st) { setup_spec(st); }

void loop_iaq(Zstate st) { loop_spec(st); }

void setup_detection(Zstate st)
{
  setup_iaq(st);
  setup_pmcounter();
}

void loop_detection(zstate *st)
{
  loop_iaq(st);
  loop_pmcounter(st);
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

    if (alertChanged())
    {
      if (currentAlert)
      {
          
        if (state.buzzerVapor==true)
        {
            //Buzzer On
            digitalWrite(buzzer, HIGH);
            VapeAlertBuzzerTime = millis();
            alertMessage=String::format("Vape alert buzzer time reset at %ul ****", VapeAlertBuzzerTime);
            debug(alertMessage + "\n");
	          VapeBuzzerOn=true;
            //delay(8000);
        }
        if (state.notifyVapor==true)
        {
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

    if (batCharge<state.batThreshold)
    {
      batCurrentAlert = true;
    }
    else
    {
      batCurrentAlert = false;
    }

    if (batAlertChanged())
    {
      if (batCurrentAlert)
      {
       if (state.notifyBattery==true)
        {
          #ifdef Version_2
          Blynk.logEvent("low_battery_alert_20");
          #else
          Blynk.email(state.batEmail, "{DEVICE_NAME} : Battery Low Alarm", "{DEVICE_NAME} battery is "+field7+"%!");
          #endif
        }
      }
      else
      {
       if (state.notifyVapor==true)
        {
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
  else 
  { 
    if (!Sensor.beginMeasuring())
    {
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


