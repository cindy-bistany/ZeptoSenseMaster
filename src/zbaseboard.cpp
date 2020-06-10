// emacs -*- c++ -*-
#include <math.h>

#include "zstate.h"
#include "zutil.h"
#include "zclock.h"
#include "zbaseboard.h"
#include "zbackhaul.h"

Zbaseboard zbaseboard;

//IoTNodePower boardPower;

///////////////////////////////////////////////////////////
// CHECK TO MAKE SURE THE EXPANDER CAN BE SEEN
// The i2c_scanner uses the return value of
// the Write.endTransmission to see if
// a device did acknowledge to the address.
//
/*
 */
void Zbaseboard::setup_expander()
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
  for (size_t i=0; i<i2cLength; ++i) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    address = i2cAddr[i];
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (!error==0) {
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
  for(address = 1; address < 127; address++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0) {
      printI2C(address);
      nDevices++;
    }
    else if (error==4) {
      debug("Unknown error at address 0x");
      if (address<16)
        debug("0");
      debug(String(address,HEX) + "\n");
    }    

  }
  
  if (nDevices == 0) debug("No I2C devices found\n");
  else debug("done\n");
}

void Zbaseboard::setup_i2c()
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
void Zbaseboard::setup()
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
  boardPower.setPowerON(EXT3V3, true);
  boardPower.setPowerON(EXT5V, true);
  v3IsOn = v5IsOn = true;
  
  setup_i2c();
}

void Zbaseboard::shutdown()
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
  power3(false);
  power5(false);
  if (!Wire.isEnabled()) Wire.end();  // Release I2C bus for expander
}

void Zbaseboard::power3(bool onoff)
{
  boardPower.setPowerON(EXT3V3, onoff);
  v3IsOn = onoff;
}

void Zbaseboard::power5(bool onoff)
{
  boardPower.setPowerON(EXT5V, onoff);
  v5IsOn = onoff;
}

bool Zbaseboard::power3IsOn() {  return v3IsOn;  }
bool Zbaseboard::power5IsOn() {  return v5IsOn;  }
float Zbaseboard::batteryLevel()	{ zbackhaul.batteryLevel(); }
void Zbaseboard::buzzer(bool onoff)	{ digitalWrite(BUZZER, onoff);  }


//Beep using Morse dots and dashes
//Make a loop to go through the bit pattern parameter
//Each bit gets a beep
//0 or . get one "dot" beep
//1 or - get 1 "dash" beep = 3 dot times
//' ' spaces get 1 dash quiet time
//There is 1 dot quiet time between dots or dashes
//
//leading to morse code here eventually
//for now beep("000111000") or (beep"...---...") will produce morse SOS
//
#define dot_length 100 //ms sound for a morse dot
#define dash_length (3*dot_length)
#define char_space dash_length
#define word_space (7*dot_length)

void Zbaseboard::beep(String s) {
  for (int i=0; i < s.length(); i++) {
    switch (s[i]) {
    case '0':
    case '.':
      buzzer(true); delay(dot_length); buzzer(false); delay(dot_length);
      break;
    case '1':
    case '-':
      buzzer(true); delay(dash_length); buzzer(false); delay(dot_length);
      break;
    case ' ':
      buzzer(false); delay(dash_length); 
      break;
    default:
      break;
    }
  }
  buzzer(false); //belt & suspenders
}



