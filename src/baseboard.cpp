/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/baseboard.ino"
// emacs -*- c++ -*-
#include <IoTNodePower.h>
#include "zstate.h"


//wake up from power off
//bring up the baseboard, but be sure all other power is turned off.
//
void setup_basehw(Zstate *st);
void shutdown_basehw(Zstate *st);
void setup_power(Zstate *st);
void setup_i2c(Zstate *st);
void setup_expander(Zstate *st);
#line 9 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/baseboard.ino"
void setup_basehw(Zstate *st)
{
  Wire.setSpeed(20000);
  // Debug console
  pinMode(D8, INPUT_PULLDOWN);
  Serial.begin(115200);
  
  Serial1.begin(115200);  // Used to monitor debug without USB connection
  delay(100);
  
  pinMode(st->portBuzzer, OUTPUT);
  digitalWrite(st->portBuzzer, LOW);
  
  Wire.begin();
  delay(100);
  debug("Start Setup section after a sleep\n");

  setup_expander(st);
  setup_power(st);
  setup_i2c(st);
  //setup_clock(st);

  st->load();
  st->readingCount=0;
  st->bInSleepMode=false;
}

void shutdown_basehw(Zstate *st)
{
  st->stateStr = "STBY";
  unsigned long elapsed = millis() - st->wakeuptime;
  st->.onTime += elapsed;

  String statusMessage = timeSynced ?
    st->stateStr + " " + Time.format(rtc.rtcNow() + gmtOffsetSeconds,"%h%e %R") + " " + field7 + "%"
    : st->stateStr + "                " + field7 + "%";
  Blynk.virtualWrite(V30,statusMessage);
  
  delay(3000);
  power.setPowerON(EXT3V3,false);
  power.setPowerON(EXT5V,false);
  // Release I2C bus for expander
  if (!Wire.isEnabled()) Wire.end();

  debug("Going to sleep\n");
  
  #if Wiring_Cellular
  if (!state.bSleepModeStandby) Cellular.off();
  #endif
  
  #if Wiring_WiFi
  WiFi.off();
  // For wifi FORCE DEEPSLEEP no stanby
  state.bSleepModeStandby=false;
  state.bInSleepMode=false;
  #endif  
}

void setup_power(Zstate *st) {
  
  power.begin();
  power.setPowerON(EXT3V3,true);
  power.setPowerON(EXT5V,true);
  // Allow time to settle
  //delay(100);
}

void setup_i2c(Zstate *st)
{
  debug("Checking i2c devices...\n");
  bool i2cOK = checkI2CDevices(i2cNames, i2cAddr, i2cLength, i2cExists);

  for (size_t x=0; x < i2cLength; x++) {
    debug(i2cNames[x]);
    debug(": ");
    debug(i2cExists[x] + "\n");
  }
  
  if (!i2cOK) {
      st->stateStr = "ERR";
      debug("I2C Issue\n");
      RGB.control(true);
      // the following sets the RGB LED to red
      RGB.color(255, 0, 0);
      delay(3000);
      // resume normal operation
      RGB.control(false);    
      //Generate 9 pulses on SCL to tell slave to release the bus 
      Wire.reset();     
      if (!Wire.isEnabled())
        Wire.begin();
      Wire.end();
    }
  else {
    st->stateStr = "RDY";
    debug("I2C OK\n");
    beep("111011101110");
  }
  delay(200);
  if (!i2cOK)
    {
      System.reset();
    }
}

#define MCP23017_ADDRESS = 0x20;

void setup_expander(Zstate *st)
{

  ///////////////////////////////////////////////////////////
  // CHECK TO MAKE SURE THE EXPANDER CAN BE SEEN
  // The i2c_scanner uses the return value of
  // the Write.endTransmisstion to see if
  // a device did acknowledge to the address.
  byte expandererror, expanderaddress;

  expanderaddress = MCP23017_ADDRESS;
  Wire.beginTransmission(expanderaddress);
  expandererror = Wire.endTransmission();

  // Try again if there is an error
  if (!expandererror==0) {
    Wire.reset();
    Wire.beginTransmission(expanderaddress);
    expandererror = Wire.endTransmission();
  }

  // Blink LED and reset if there is an error
  if (!expandererror == 0) {
    debug("Unable to read Expander - resetting in 9 seconds\n");
    Particle.publish("Error","Unable to read Expander - resetting in 9 seconds",PRIVATE);
    blinkRed.setActive(true);
    delay(3000);
    blinkRed.setActive(false);
    blinkYellow.setActive(true);
    delay(3000);
    blinkYellow.setActive(false);
    blinkRed.setActive(true);
    delay(3000);
    System.reset();
  }
}
