// emacs -*- c++ -*-
#include <math.h>
#include <IoTNodePower.h>

#include "zstate.h"
#include "util.h"
#include "zclock.h"
#include "baseboard.h"

//IoTNodePower boardPower;

///////////////////////////////////////////////////////////
// CHECK TO MAKE SURE THE EXPANDER CAN BE SEEN
// The i2c_scanner uses the return value of
// the Write.endTransmission to see if
// a device did acknowledge to the address.
//
/*
Baseboard::Baseboard()
{
}
*/

void Baseboard::setup_expander()
{
  const byte MCP23017_ADDRESS = 0x20;
  byte expanderaddress = MCP23017_ADDRESS;
  byte expandererror;

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
    blinkpanic();
    System.reset();
  }
}
////////////////
//  I2C functions
//////////////

String i2cNames[] = {
    "RTC",
    "Exp",
    "RTC EEPROM",
    "ADC",
    "FRAM",
    "ADXL345",
    "SPS30"
};

byte i2cAddr[]= {
    0x6F, //111
    0x20, //32
    0x57, //87
    0x4D, //77
    0x50, //80
    0x53, //83
    0x69 //105
    
};

// number of elements in `array`
static const size_t i2cLength = sizeof(i2cAddr) / sizeof(i2cAddr[0]);

bool i2cExists[]=
{
  false,
  false,
  false,
  false,
  false,
  false,
  false
};

// check i2c devices with i2c names at i2c address of length i2c length returned in i2cExists
bool checkI2CDevices(String i2cNames[], byte i2cAddr[], size_t i2cLength, bool i2cExists[])
{
  byte error, address;
  bool result = true;
  for (size_t i=0; i<i2cLength; ++i)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    address = i2cAddr[i];
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (!error==0)
    {
      Wire.reset();
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
    }

    String msg = "Device "+ i2cNames[i] + " address:0x" + String(address, HEX);
    result = i2cExists[i] = (error == 0);
    msg += result ? " FOUND\n" : " NOT FOUND\n";
    debug(msg);
  }
  return result;
}


void printI2C(int inx)
{
    for (int i=0; i<i2cLength; i++)
      if (i2cAddr[i] == inx)
	debug(String("Device "+i2cNames[i]+ " at"+" address:0x"+String(i2cAddr[i], HEX) + "\n"));
}

void scanI2C()
{
  byte error, address;
  int nDevices;
 
  debug("Scanning...\n");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      printI2C(address);
      nDevices++;
    }
    else if (error==4)
    {
      debug("Unknown error at address 0x");
      if (address<16)
        debug("0");
      debug(String(address,HEX) + "\n");
    }    
  }
  if (nDevices == 0)
    debug("No I2C devices found\n");
  else
    debug("done\n");
}

void Baseboard::setup_i2c()
{
  debug("Checking i2c devices...\n");
  bool i2cOK = checkI2CDevices(i2cNames, i2cAddr, i2cLength, i2cExists);

  for (size_t x=0; x < i2cLength; x++) {
    debug(i2cNames[x]);
    debug(": ");
    debug(i2cExists[x] + "\n");
  }
  
  if (!i2cOK) {
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
    debug("I2C OK\n");
    beep("111011101110");
  }
  delay(200);
  if (!i2cOK) System.reset();
}


//wake up from power off
//bring up the baseboard, but be sure all other power is turned off.
//
#define BUZZER D7
void Baseboard::setup()
{
  beep("---");
  
  Wire.setSpeed(20000);
  // Debug console
  pinMode(D8, INPUT_PULLDOWN);
  Serial.begin(115200);
  
  Serial1.begin(115200);  // Used to monitor debug without USB connection
  delay(100);
  
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  
  Wire.begin();
  delay(100);
  debug("Start Setup section after a sleep\n");

  setup_expander();
  
  boardPower.begin();
  boardPower.setPowerON(EXT3V3, false);
  boardPower.setPowerON(EXT5V, false);
  v3IsOn = v5IsOn = false;
  
  setup_i2c();
  //setup_clock(st);

}

//
// Battery functions
//
bool Baseboard::batteryIsCharged()
{
  beep("...---...");
  return false;
}

FuelGauge fuel;

float Baseboard::batteryLevel()
{
#if Wiring_Cellular
  return fuel.getSoC();
#endif

#if Wiring_WiFi
  float voltage = analogRead(BATT) * 0.0011224;
  float batCharge = exp(5.0601*voltage)*0.0000001;
  if (batCharge>100) batCharge = 100;
  return batCharge;
#endif
  beep("...---...  ...---...");
  return -1;
}
//////////////


float Baseboard::signalStrength()
{
#if Wiring_Cellular
    CellularSignal sig = Cellular.RSSI();
#endif

#if Wiring_WiFi
    WiFiSignal sig = WiFi.RSSI();
#endif
    return sig;
}

void Baseboard::buzzer(bool onoff)
{
  digitalWrite(BUZZER, onoff);
}

void Baseboard::shutdown()
{
  /*  st->stateStr = "STBY";
  unsigned long elapsed = millis() - st->wakeuptime;
  st->onTime += elapsed;
  */
  
  /*  String statusMessage = st->timeIsSynchronized ?
    st->stateStr + " " + Time.format(clocknow() + st->gmtOffsetSeconds,"%h%e %R") + " " + batteryIsCharged() + "%"
    : st->stateStr + "                " + batteryLevel() + "%";
  blynk_status_message(statusMessage);
  */
  
  delay(3000);
  powerOnOff(0, false);
  if (!Wire.isEnabled()) Wire.end();  // Release I2C bus for expander

  debug("Going to sleep\n");
  
  #if Wiring_Cellular
  if (!st->bSleepModeStandby) Cellular.off();
  #endif
  
  #if Wiring_WiFi
  WiFi.off();
  // For wifi FORCE DEEPSLEEP no stanby
  /*
  st->bSleepModeStandby=false;
  st->bInSleepMode=false;
  */
  #endif  
}

void Baseboard::powerOnOff(int which, bool onoff)
{
  switch (which) {
  case 3:
    boardPower.setPowerON(EXT3V3, onoff);
    v3IsOn = onoff;
    break;
  case 5:
    boardPower.setPowerON(EXT5V, onoff);
    v5IsOn = onoff;
    break;
  default:
    boardPower.setPowerON(EXT3V3, onoff);
    boardPower.setPowerON(EXT5V, onoff);
    v3IsOn = v5IsOn = onoff;
  }
}

int Baseboard::powerIsOnOff()
{
  int ret = 0;
  if (v3IsOn) ret |= 1;
  if (v5IsOn) ret |= 2;
  return ret;
}




