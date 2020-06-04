/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/i2c.ino"

bool checkI2CDevices(String i2cNames[], byte i2cAddr[], size_t i2cLength, bool i2cExists[]);
void printI2C(int inx);
void scanI2C();
#line 2 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/i2c.ino"
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
