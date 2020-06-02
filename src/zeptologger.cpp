/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/zeptologger.ino"
// Particle.io (Arduino) code for zeptive vape sensor
// Copyright 2020 zeptive.com
//

void beep(unsigned int bp);
bool checkI2CDevices(String i2cNames[], byte i2cAddr[], size_t i2cLength, bool i2cExists[]);
void printI2C(int inx);
void scanI2C();
void deepSleep();
void setAppWidgets();
void initializeAppWidgets();
bool runExpression(char *expression);
bool alertChanged();
bool batAlertChanged();
bool tamperAlertChanged();
void gmtOffsetHandler(const char *event, const char *data);
void timerSleep(long seconds);
void setAlarm(int DURATION);
void initializeAlarm();
void activateAlarmPowerdown();
void connect();
void connectWithoutWaiting();
bool connected();
void publishGMTOffsetRequest();
#line 5 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/zeptologger.ino"
#define BLYNK_PRINT Serial1 // Defines the object that is used for printing
#define BLYNK_DEBUG        // Optional, this enables more detailed prints
#define BLYNK_HEARTBEAT      60

unsigned long CycleOnTime=0;
unsigned long VapeAlertTime=0;
bool VapeBuzzerOn=false;
unsigned long VapeAlertBuzzerTime=0;
uint16_t portBlynk = 8080;

#include <IoTNodePower.h>
#define USEINT // Turn on if using interrupts for accelerometer

#include <SPS30.h>

#include "ULP.h"

#include "blynk.h"

#include "tinyexpr.h"

#include <MB85RC256V-FRAM-RK.h>

#include "MCP7941x.h"

#define SERIAL_DEBUG
#define KURTDEBUG

#ifdef SERIAL_DEBUG
  #define debug(...) Serial1.print(__VA_ARGS__)
  #define debugf(...) Serial1.printf(__VA_ARGS__)
#else
  #define debug(...)
  #define debugf(...)
#endif

void beep(unsigned int bp) {
  //make a loop to go through the bit pattern parameter
  //each bit gets a beep
  //0 bits get one beep quantum, 1 bits get 3 beep quanta
  //1 quantum between bits
  //delete leading zeroes, replace with 6 beep quanta of quiet
  //
  //leading to morse code here eventually
  //
  digitalWrite(buzzer, HIGH);
  delay(5);
  digitalWrite(buzzer, LOW);
  delay(200);  
  digitalWrite(buzzer, HIGH);
  delay(5);
  digitalWrite(buzzer, LOW);
  delay(200);  
  digitalWrite(buzzer, HIGH);
  delay(5);
  digitalWrite(buzzer, LOW);
}

// Uncomment for startup beeps
// Two pips okay
// One beep not okay
#define BEEP

//***************** INTERRUPT ******************
int interruptPin = A0;                 // Setup pin A0 for Boron

#ifdef PARTICLE
#else
  #include "arduino.h"
  #include <Wire.h>  
#endif //end of #ifdef PARTICLE

#define Version_2


#ifdef Version_2
#define alertRedImage 1
#define alertGreenImage 0
#else
#define alertRedImage 2
#define alertGreenImage 1
#endif

// SYSTEM_MODE(SEMI_AUTOMATIC);
// SYSTEM_THREAD(ENABLED);

// EXAMPLE - defining and using a LED status
LEDStatus blinkYellow(RGB_COLOR_YELLOW, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

LEDStatus blinkRed(RGB_COLOR_RED, LED_PATTERN_BLINK, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);


MB85RC256V fram(Wire, 0);


typedef struct {
  float mass_concen[4];
  float num_concen[5];
} Readings;

bool powerOn, appConnected, sensorValid = false, currentAlert = false, terminalDebug = false;
bool batCurrentAlert = false, tamperCurrentAlert = false;

IoTNodePower power;

FuelGauge fuel;

float mass_concen[4];
float num_concen[5];

char *pm[5] = {"PM0.5", "PM1.0", "PM2.5", "PM4.0", "PM10"};

int i=0;
unsigned long mytime;
int readingCount = 0;
int buzzer = D7;
int gmtOffsetSeconds = -14400;
// Flag to check if the offset has been updated
bool gmtOffsetValid = false;
// Flag to check if the time has been synced
bool timeSynced = false;

String field1,field2,field3,field4,field5,field6,field7,field8;
String datastring;

// Flag to keep track of accel interrupts
bool accelInterrupt = false;

// Install Information Set to 1 to configure the correct particle board
//#define NM01
#define NM02 0
#define NM03 0
#define NM04 0
#define NM05 0
#define NM06 0
#define NM07 0
#define NM08 0
#define NM09 0
#define NM10 0
#define NM11 0
#define NM12 0
#define NM13 0
#define NM14 0
#define NM15 0
#define NM16 0
#define NM17 0
#define NM18 0
#define NM19 0
#define NM20 0
#define MH01 0
#define MH02 0
#define CH01 0
#define CH02 0
#define DEMO1 0
#define DEMO2 0 

#ifdef NM01
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190001W ";
char auth[] = "TW3w4ipRRSzlWn5fLzN0HLyrERrYc1TK"; // Auditorium - Boys #1
String InstallLocation = " Loc = Auditorium - Boys #1  ";
#elif NM02
char auth[] = "oB6u2NFryk87-WKWpitb4f5sJEPVW5Qi";  // Auditorium - Boys #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190002 ";
String InstallLocation = " Loc = Auditorium - Boys #2 ";
#elif NM03
char auth[] = "wtHMksk1Vk-XG9vtrQ06R7Uywjpzn8C3";  // Auditorium - Boys #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190003 ";
String InstallLocation = " Loc = Auditorium - Boys #3 ";
#elif NM04 
char auth[] = "4AXE6GzownAQO6ryqIhh0XxS3Rkwpv77";  // Auditorium - Girls #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190004W ";
String InstallLocation = " Loc = Auditorium - Girls #1 ";
#elif NM05
char auth[] = "JnEYsIRf6uh_tMGvI1lFW6I7YvN-V3II";  // Auditorium - Girls #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190005W ";
String InstallLocation = " Loc = Auditorium - Girls #2 ";
#elif NM06
char auth[] = "sk0SVmeo2aqxjrI2rcXD_Lz86aMl_RGT";  // Auditorium - Girls #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190006W ";
String InstallLocation = " Loc = Auditorium - Girls #3 ";
#elif NM07
char auth[] = "pz6vWIMHyYtMphoQg8xLKqxgP3OGeTWY";  // Cafeteria - Boys #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190007W ";
String InstallLocation = " Loc = Cafeteria - Boys #1 ";
#elif NM08
char auth[] = "uxnWrECLKCmo_W3ybm5qFrQ6Sm1Yp4a3";  // Cafeteria - Boys #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190008W ";
String InstallLocation = " Loc = Cafeteria - Boys #2 ";
#elif NM09
char auth[] = "Rmlj1GhVlS54AUz5QtVZTQto7hwQiFHY";  // Cafeteria - Boys #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190009W ";
String InstallLocation = " Loc = Cafeteria - Boys #3 ";
#elif NM10
char auth[] = "Sc65ul1ZhwP-PXUASHu0f6w6ECESQtYk";  // Cafeteria - Girls #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190010W ";
String InstallLocation = " Loc = Cafeteria - Girls #1 ";
#elif NM11
char auth[] = "4iZj-BzJnEhb3vQdcMMv_UKeRqIu0q1l";  // Cafeteria - Girls #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190011W ";
String InstallLocation = " Loc = Cafeteria - Girls #2 ";
#elif NM12
char auth[] = "HM6P8AQSXlivj7qyx6PoP1XLA3N6kR9P";  // Cafeteria - Girls #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190012W ";
String InstallLocation = " Loc = Cafeteria - Girls #3 ";
#elif NM13
char auth[] = "f5sjoGvVioMghFn2ogo5DgZa9TWm8Wyl";  // 2nd Floor - Boys #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190013W ";
String InstallLocation = " Loc = 2nd Floor - Boys #1 ";
#elif NM14
char auth[] = "9JG4IXAL4YAtKhaeB1gny4ccVOA4Zfag";  // 2nd Floor - Boys #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190014W ";
String InstallLocation = " Loc = 2nd Floor - Boys #2 ";
#elif NM15
char auth[] = "ZBw9G8640LttTvMUMLQBhe5RGlpDCZrl";  // 2nd Floor - Boys #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190015W ";
String InstallLocation = " Loc = 2nd Floor - Boys #3 ";
#elif NM16
char auth[] = "IpJUfUT5y9ddjrKIq6IxX0coDerLSA4W";  // 2nd Floor - Girls #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190016 ";
String InstallLocation = " Loc = 2nd Floor - Girls #1 ";
#elif NM17
char auth[] = "5k-Ybn5b3cvoiT9cpXs1ea97IqEJmEbN";  // 2nd Floor - Girls #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190017 ";
String InstallLocation = " Loc = 2nd Floor - Girls #2 ";
#elif NM18
char auth[] = "ByGW-6MlpEmDyNplY570GWaPjlAa8vfb";  // 2nd Floor - Girls #3
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190018W ";
String InstallLocation = " Loc = 2nd Floor - Girls #3 ";
#elif NM19
char auth[] = "GFqcx624QTCu4JG6JK_QbV830J5zF6M0";  // Portable #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190019W ";
String InstallLocation = " Loc = Portable #1 ";
#elif NM20
char auth[] = "T2lc5d1JT7T70q-1ukKVnHq0YQ0fOQjo";  // Portable #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = NM_10012190020W ";
String InstallLocation = " Loc = Portable #2 ";
#elif MH01 
char auth[] = "Hzk-5ToU8Za1TUhirSOwewS5DaHsJMzi";  // Melrose #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = MHS_12002200001W ";
String InstallLocation = " Loc = Boys #1 ";
#elif MH02
char auth[] = "JRExJiLEcxwJfXaC3fIhRcWXybpSTGM2";  // Melrose #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = MHS_12002200002W ";
String InstallLocation = " Loc = Girls #1 ";
#elif CH01 
char auth[] = "W8Z09poq6n7wTGKUAi_fzl5Gv8HRFwT9";  // Chelmsford #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = CHS_10002200001 ";
String InstallLocation = " Loc = Sensor #1 ";
#elif CH02
char auth[] = "ydW-C263BLpJPl7U7s8_uyDrX-wlmRmf";  // Chelmsford #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = CHS_10002200002 ";
String InstallLocation = " Loc = Sensor #2 ";
#elif DEMO1
char auth[] = "AU7PDy_yGZjGSV9DwINUIustqPzeIw-b";  // Zeptive Demo #1
String iSoftwareVersion = " SW Ver = 2019112 S/N = ZepDemo_001 ";
String InstallLocation = " Stoneham  ";
#elif DEMO2
char auth[] = "utaIzfAalhOaa76maecfcBefW0SRm26U";  // Zeptive Demo #2
String iSoftwareVersion = " SW Ver = 2019112 S/N = ZepDemo_002 ";
String InstallLocation = " Wilmington ";
#else 
// char auth[] = "xDQv4KE4ZKS7DW8ba4OuwtY-XIXeVvYT"; //Bathroom Test
// char auth[] = "2VbD53DCkxcnlyu28w_KL9TWpdOom1nQ"; //Bathroom 1
//char auth[] = "JouQtmYm-Cs2QCjZA18AxFAvdFrs6Sr6"; //Bathroom 2
//char auth[] = "IODR4bIAgZKqMptIpYHIvpLx3L8qmLfS"; //Bathroom 2 (New)
//char auth[] = "Z8lveVS4uGA9G2iVjKZQKUEwJf5ZWClf "; //Bathroom 2 (Blynk 2 new)
// char auth[] = "2DNcMB9D3Q9IMJAHkHl0NqyCtE326uGm"; //Bathroom 3
// char auth[] = "wjKKdx0kjzS_33wY3C4CCtOJRn8Lfh3D"; //Bathroom 4
char auth[] = "pUSnK9eAbHcgOAi_v1vN1cPYUIID6Iwv"; //Bathroom 5 (Blynk 2 new)
String iSoftwareVersion = " SW Ver = 2019116 S/N = Zep2W ";
String InstallLocation = " Loc = Test Unit ";
#endif
//  char auth[] = "q4ey21hbwczzBHyRj_Z-f7EJieP491Qu"; // Sentient Things Test 2.0
 // I8213EIuNfVpyoW6dITCAy4glOI7s7Fs

String StateString = "RDY";

BlynkTimer sensorTimer; // Create a Timer object called "sensorTimer"!
BlynkTimer accelTimer; // Create an accelerometer Timer object

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
///////////////////////

  // ULP - Library for reading SPEC Sensors ULP.
  // Created by David E. Peaslee, OCT 27, 2017.
  // Released into the SPEC Sensors domain.


  // The library is built to use several SPEC ULP sensors. An Arduino Uno can handle 3 sensors, since 2 analog pins are required for each sensor.
  // Functions include getTemp(seconds, C/F): returns temperature, setTSpan(seconds, "HIGH"/"LOW"): for calibrating temperature, setVref(R1,R2,R3) for custom sensors,
  // getVgas(seconds): for getting voltage of sensor, getConc(seconds, temperature in degC): for getting temperature corrected concentration,
  // setXSpan(): for setting the calibration factor with known concentration of gas. Variable include _Vcc: the voltage ref of the ADC, _Vsup: the voltage to the ULP, 
  // and _Gain the value of resistor R6. For more details see ULP.cpp in the libraries folder and the data sheet at http://www.spec-sensors.com/wp-content/uploads/2016/06/Analog-SDK-Users-Manual-v18.pdf


// As an example, the ULP is connected this way: Vgas (pin1) to A0, Vtemp (Pin3) to A3, Gnd (Pin6) to Gnd, V+ (Pin7 or Pin8) to 3.3V of Arduino (must not be above 3.3V!!!).

// These constants won't change.  They're used to give names to the pins used and to the sensitivity factors of the sensors:

//const int C1 = A2;
//const int T1 = A4;

const float Sf1 = 4.05; //nA/ppm replace this value with your own sensitivity


float conc1, temp1;
float TZero;
float Vzero1;

IAQ sensor1(A2, A4, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//IAQ sensor1(C1, T1, Sf1);  //Sensor Types are EtOH, H2S, CO, IAQ, SO2, NO2, RESP, O3, and SPEC (custom)
//O3 sensor2(C2, T2, Sf2);  //Example O3
//H2S sensor3(C3, T3, Sf3); //Example H2S

//  Include these if using different boards with different voltages
//float ULP::_Vcc = 5.0;  //analogRead Reference Voltage
//float ULP::_Vsup =3.3;  //voltage supplied to V+ of ULP, ideally 3.3 Volts 

///////////////////////

readhw();

// BLYNK_WRITE is a function called every time device gets an update of Virtual Pin value from the server (or app):
// Get the desired number of readings from the app
BLYNK_WRITE(V10)
{   
  state.numberOfReadings = param.asInt(); // Get value as integer
  //Save
  saveState();
  setAppWidgets();
}

BLYNK_WRITE(V12)
{   
  state.secondsBetweenReadings = param.asInt(); // Get value as integer
  //Save to FRAM
  saveState();
  setAppWidgets();
}

void deepSleep()
{
  String statusMessage;
  StateString = "STBY";
  unsigned long elapsed = millis() - CycleOnTime;
  state.OnTime=state.OnTime+elapsed;
  //Save
  saveState();

  if (timeSynced)
  {
       statusMessage = StateString+" "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R")+" "+field7+"%";

  }
  else
  {
       statusMessage = StateString+"                "+field7+"%";
  }
  Blynk.virtualWrite(V30,statusMessage);
  delay(3000);
  power.setPowerON(EXT3V3,false);
  power.setPowerON(EXT5V,false);
  // Release I2C bus for expander
  if (!Wire.isEnabled()) {
    Wire.end();
  }  

  debug("Going to sleep\n");
  #if Wiring_Cellular
  if (!state.bSleepModeStandby)
    Cellular.off();
  #endif
  #if Wiring_WiFi
  WiFi.off();
  // For wifi FORCE DEEPSLEEP no stanby
  state.bSleepModeStandby=false;
  state.bInSleepMode=false;
  #endif  

  delay(1000);
  if (state.bSleepModeStandby){
    debug("Going to standby sleep\n");
    state.bInSleepMode=true;
    System.sleep(D8, RISING, 900, SLEEP_NETWORK_STANDBY);
    return;
  }
  else{
    debug("Going to deep sleep\n");
    state.bSleepModeStandby=false;
    state.bInSleepMode=true;
    delay(2000);
    System.sleep(SLEEP_MODE_DEEP); 
    }
}

// Update app connection state
BLYNK_APP_CONNECTED() {
  appConnected = true;
  setAppWidgets();
  debug("Connected\n");
}

BLYNK_APP_DISCONNECTED() {
// Your code here
 appConnected = false;
 debug("Disconnected\n");
}

// Update the App UI widgets as needed
void setAppWidgets()
{
  Blynk.virtualWrite(V11, state.numberOfReadings);
  Blynk.virtualWrite(V13, state.secondsBetweenReadings);
  Blynk.virtualWrite(V15, state.zeroOff);
}

void initializeAppWidgets()
{
  Blynk.virtualWrite(V10, state.numberOfReadings);
  Blynk.virtualWrite(V12, state.secondsBetweenReadings);
  Blynk.virtualWrite(V11, state.numberOfReadings);
  Blynk.virtualWrite(V13, state.secondsBetweenReadings);
  Blynk.virtualWrite(V20, state.expression);
  Blynk.virtualWrite(V16, state.email);
  Blynk.virtualWrite(V18, state.batEmail);
  Blynk.virtualWrite(V26, state.tamperEmail);
  Blynk.virtualWrite(V15, state.zeroOff);
  Blynk.virtualWrite(V25, state.batThreshold);
  Blynk.virtualWrite(V23, state.deviceZone);
 // Blynk.virtualWrite(V24, state.buzzerActivated);
  Blynk.virtualWrite(V22,"Enter code to reset");
  // New Pins
  Blynk.virtualWrite(V24, state.buzzerTamper); // Buzzer Tamper
  Blynk.virtualWrite(V27, state.buzzerVapor); // Vapor Buzzer
  Blynk.virtualWrite(V28, state.notifyVapor); // Vapor Notify
  Blynk.virtualWrite(V29, state.notifyTamper); // Tamper Notify
  Blynk.virtualWrite(V32, state.notifyBattery); // Battery Notify
  if (state.ActivityThreshold==144)
  	Blynk.virtualWrite(V31, 3); // Tamper Setting Hammer
  else  if (state.ActivityThreshold==72)
  	Blynk.virtualWrite(V31, 1); // Tamper Setting feather
  else
  	Blynk.virtualWrite(V31, 2); // Tamper Setting default

  if (terminalDebug) Blynk.virtualWrite(V21, "Initialized default values");
  state.firstRunCheck = firstRunValue;
  state.bSleepModeStandby=true;
  state.bInSleepMode=false;
  debug("Finished Initialization\n");
  saveState();
}

BLYNK_WRITE(V22) //Reset Expression
{
  String _resetKey = param.asStr();
  if (_resetKey.equals("resetme"))
  {
    Blynk.virtualWrite(V22,"Enter code to reset");
    state.numberOfReadings = 15;
    state.secondsBetweenReadings=5;
    state.firstRunCheck = 0;
    state.bSleepModeStandby=true;
    state.bInSleepMode=false;
    state.zeroOff = 0;
    strcpy(state.expression,"Enter expression here");
    strcpy(state.email,"Enter email here");
    strcpy(state.batEmail,"Enter email here");
    strcpy(state.tamperEmail,"Enter email here");
    state.deviceZone=0;
    state.batThreshold = 20;
    state.lastAlert=false;
    state.batLastAlert=false;
    state.ActivityThreshold=100; // Which is set to 1 in Blynk
    state.buzzerTamper=true;  // Buzzer Tamper
    state.buzzerVapor=false; // Vapor Buzzer
    state.notifyVapor=true; // Vapor Notify
    state.notifyTamper=true; // Tamper Notify
    state.notifyBattery=true; // Battery Notify
    state.OnTime=0;
    Blynk.virtualWrite(V10, state.numberOfReadings);
    Blynk.virtualWrite(V12, state.secondsBetweenReadings);
    Blynk.virtualWrite(V11, state.numberOfReadings);
    Blynk.virtualWrite(V13, state.secondsBetweenReadings);
    Blynk.virtualWrite(V20, state.expression);
    Blynk.virtualWrite(V16, state.email);
    Blynk.virtualWrite(V18, state.batEmail);
    Blynk.virtualWrite(V26, state.tamperEmail);
    Blynk.virtualWrite(V15, state.zeroOff);
    Blynk.virtualWrite(V25, state.batThreshold);
    Blynk.virtualWrite(V23, state.deviceZone);
    Blynk.virtualWrite(V24, state.buzzerTamper);  // Buzzer Tamper
    Blynk.virtualWrite(V27, state.buzzerVapor); // Vapor Buzzer
    Blynk.virtualWrite(V28, state.notifyVapor); // Vapor Notify
    Blynk.virtualWrite(V29, state.notifyTamper); // Tamper Notify
    Blynk.virtualWrite(V31, 2); // Which is 100 for the accelerometer
    Blynk.virtualWrite(V32, state.notifyBattery); // Battery Notify
    // EEPROM.clear();
    fram.erase();
    Blynk.virtualWrite(V21, "Hard Resetting");
    debug("Hard Resetting\n");
    delay(500);
    System.reset();
  }
}

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  // Request Blynk server to re-send latest values for all pins
  debug("Blynk is now connected - syncing all pins.\n");
  if (state.firstRunCheck != firstRunValue)
  {
    initializeAppWidgets();
  }
  Blynk.syncAll();
  // You can also update individual virtual pins like this:
  //Blynk.syncVirtual(V0, V2);
}

BLYNK_WRITE(V20) //Alarm Expression
{
  String _expression = param.asStr();
  //Only lower case is allowed
  _expression.toLowerCase();
  strcpy(state.expression,_expression.c_str());
  saveState();
  if (sensorValid)
  {
    runExpression(state.expression);
  }
}

BLYNK_WRITE(V16) //Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
  {
    strcpy(state.email,_email.c_str());
    saveState();
  }
  else
  {
    Blynk.virtualWrite(V16, "Invalid address");
  }
  
}

BLYNK_WRITE(V18) //Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
  {
    strcpy(state.batEmail,_email.c_str());
    saveState();
  }
  else
  {
    Blynk.virtualWrite(V18, "Invalid address");
  }
  
}

BLYNK_WRITE(V26) //Email Expression
{
  String _email = param.asStr();
  //Add validation later
  bool checkEmail = true;
  _email.toLowerCase();
  if (checkEmail)
  {
    strcpy(state.tamperEmail,_email.c_str());
    saveState();
  }
  else
  {
    Blynk.virtualWrite(V26, "Invalid address");
  }
  
}
// Calibration
BLYNK_WRITE(V14) 
{
  if (param.asInt()==1)
  {
    state.zeroOff = sensor1.zero();
    saveState();
    Blynk.virtualWrite(V15, state.zeroOff);
  }
  Blynk.virtualWrite(V14, 0);
  // Since data will be invalid save the new calibration and reset
  delay(4000);
  System.reset();
}

//Battery threshold
BLYNK_WRITE(V25) 
{
  float thresh = param.asFloat();
  state.batThreshold=thresh;
  saveState();
}

BLYNK_WRITE(V17) 
{
  #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
  #endif
  if (param.asInt()==1)
  {
    terminalDebug = false;
  }
  else
  {
    terminalDebug = true;
  }
}

// Update time zone
BLYNK_WRITE(V23) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    state.deviceZone=param.asInt();
#ifdef KURTDEBUG
    debug("*****Device Zone value is: ");
    debug(state.deviceZone + "\n");
#endif
    gmtOffsetValid=false;
  saveState();
}
// Update buzzerTamper flag
BLYNK_WRITE(V24) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    state.buzzerTamper=param.asInt();
  saveState();

#ifdef KURTDEBUG
    debug("*****Update buzzerTamper flag value is: ");
    debug(state.buzzerTamper + "\n");
#endif
}
// Update buzzerVapor flag
BLYNK_WRITE(V27) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    state.buzzerVapor=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("*****Update buzzerVapor flag value is: ");
    debug(state.buzzerVapor + "\n");
#endif
}
// Update notifyVapor flag
BLYNK_WRITE(V28) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    state.notifyVapor=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("*****Update notifyVapor flag value is: ");
    debug(state.notifyVapor + "\n");
#endif
}
// Update notifyTamper flag
BLYNK_WRITE(V29) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    state.notifyTamper=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("*****Update notifyTamper flag value is: ");
    debug(state.notifyTamper + "\n");
#endif
}

//Accelerometer threshold
BLYNK_WRITE(V31) 
{
  int thresh = param.asInt();
  int oldvalue=state.ActivityThreshold;

#ifdef KURTDEBUG
    debug("*****Update Accelerometer threshold value is: ");
    debug(thresh + "\n");
#endif
  state.ActivityThreshold=100;
  if (thresh==3)
  	state.ActivityThreshold=144;
  if (thresh==2)
  	state.ActivityThreshold=100;
  if (thresh==1)
  	state.ActivityThreshold=72;
  // If value changed since the acceleromter will need to be reinitalized save the new value and reset
  if (state.ActivityThreshold != oldvalue)
  {
#ifdef KURTDEBUG
    debug("Update Accelerometer value is: ");
    debug(state.ActivityThreshold + "\n");
    debug("Will reset system ");
#endif
    saveState();
    delay(4000);
    System.reset();
  }
}

// Update notifyBattery flag
BLYNK_WRITE(V32) 
{
    #ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    delay(200);  
    digitalWrite(buzzer, HIGH);
    delay(5);
    digitalWrite(buzzer, LOW);
    #endif
    state.notifyBattery=param.asInt();
  saveState();
#ifdef KURTDEBUG
    debug("Update notifyBattery flag value is: ");
    debug(state.notifyBattery + "\n");
#endif
}
bool runExpression(char *expression)
{   
  double pm1, pm2, pm4, pm10, conc, temp;
  // Store variable names and pointers. 
  te_variable vars[] = {{"pm1", &pm1}, {"pm2", &pm2}, {"pm4", &pm4}, {"pm10", &pm10}, {"conc", &conc}, {"temp", &temp}};
  if (terminalDebug) Blynk.virtualWrite(V21, expression); 
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

// Checks if alert status changed and resets lastAlert if it has
bool alertChanged()
{
  if (currentAlert==state.lastAlert)
  {
    return false;
  }
  else
  {
    state.lastAlert=currentAlert;
    saveState();
    return true;
  }
}

// Checks if alert status changed and resets lastAlert if it has
bool batAlertChanged()
{
  if (batCurrentAlert==state.batLastAlert)
  {
    return false;
  }
  else
  {
    state.batLastAlert=batCurrentAlert;
    saveState();
    return true;
  }
}

// Checks if tamper alert status changed 
bool tamperAlertChanged()
{
  if (tamperCurrentAlert==state.tamperLastAlert)
  {
    return false;
  }
  else
  {
    state.tamperLastAlert=tamperCurrentAlert;
    saveState();
    return true;
  }
}

void gmtOffsetHandler(const char *event, const char *data) {
  // Handle the integration response
  gmtOffsetSeconds=atoi(data);
  gmtOffsetValid = true;
  debug("GMT seconds offset is: ");
  debug(gmtOffsetSeconds + "\n");
}


void timerSleep(long seconds)
{
  String statusMessage;
  StateString = "STBY";
  if (timeSynced)
  {
       statusMessage = StateString+" "+Time.format(rtc.rtcNow()+gmtOffsetSeconds,"%h%e %R")+" "+field7+"%";
  }
  else
  {
       statusMessage = StateString+"                "+field7+"%";
  }
  Blynk.virtualWrite(V30,statusMessage);
    setAlarm(seconds);
    activateAlarmPowerdown();
    delay(200);  
}


void setAlarm(int DURATION)
{
    rtc.disableAlarm0();
    int rtcnow = rtc.rtcNow();
    debug(rtcnow + "\n");
    int alarmTime = rtcnow + DURATION;
    String mess = String(Time.format(rtcnow, TIME_FORMAT_ISO8601_FULL))+" + "+String(DURATION)+" = "+String(Time.format(alarmTime, TIME_FORMAT_ISO8601_FULL))+"\r\n";
    debug(mess);
    if (terminalDebug) Blynk.virtualWrite(V21, mess);
    rtc.setAlarm0UnixTime(alarmTime);
}

void initializeAlarm()
{
  debug("Initializing alarm\n");
  rtc.outHigh();
  rtc.disableAlarms();
  rtc.maskAlarm0("Seconds");
  rtc.setAlarm0PolHigh();
  rtc.clearIntAlarm0();
}

void activateAlarmPowerdown()
{
    // Clear the alarm0 interrupt
    rtc.clearIntAlarm0();
 
    // Enable the alarm. This will set the MFP output low
    // turning off the power until the alarm triggers
    // turning on the power again and starting the code from
    // the beginning.
    rtc.enableAlarm0();
 rtc.publishAlarm0Debug();
    
}

// Adding explicit connect routine that has to work before the rest of the code runs
void connect()
{
  #if Wiring_Cellular
  bool cellready=Cellular.ready();
  if (!cellready)
  {
    debug("Attempting to connect cellular...\n");
    Cellular.on();
    Cellular.connect();
    waitFor(Cellular.ready,180000);
    if (!Cellular.ready())
    {
    debug("Cellular not ready\n");
    delay(200);
    // timerSleep(3);
    System.reset();
    }
  }
  else
  {
    debug("Cellular ready\n");
  }
  #endif
  #if Wiring_WiFi
  bool cellready=WiFi.ready();
  if (!cellready)
  {
    debug("Attempting to connect WiFi...\n");
    WiFi.on();
    WiFi.connect();
    waitFor(WiFi.ready,180000);
    if (!WiFi.ready())
    {
    debug("WiFi not ready\n");
    delay(200);
    // timerSleep(3);
    System.reset();
    }
  }
  else
  {
    debug("WiFi ready\n");
  }
  #endif  
//*** Commented out for testing without particle connected ***
  bool partconnected=Particle.connected();
  if (!partconnected)
  {
    debug("Attempting to connect to Particle...\n");
    Particle.connect();
    waitFor(Particle.connected,60000);
    if (!Particle.connected())
    {
      debug("Particle not connected\n");
      delay(200);
      // timerSleep(3);
      System.reset();
    } 
  }
  else
  {
    debug("Particle connected\n");
  }
//*** Commented out for testing without particle connected ***
  bool blynkconnected=Blynk.connected();
  if (!blynkconnected)
  {
    debug("Attempting to connect to Blynk...\n");
    #ifdef Version_2
    Blynk.config(auth, "zeptosense2.blynk.cc",portBlynk);
    #else
    Blynk.config(auth, "zeptosense.blynk.cc");
    #endif
    if (!Blynk.connect())
    {
      debug("Blynk not connected\n");
      delay(200);
      // timerSleep(3);
      System.reset();      
    }
  }
  else
  {
    debug("Blynk connected\n");
  }
}

void connectWithoutWaiting()
{
  #if Wiring_Cellular
  bool cellready=Cellular.ready();
  if (!cellready)
  {
    debug("Cellular not ready\n");
    if (!Cellular.connecting())
    {
      debug("Attempting to connect cellular...\n");
      Cellular.on();
      Cellular.connect();
    }
  }
  #endif
  #if Wiring_WiFi
  bool wifiready=WiFi.ready();
  if (!wifiready)
  {
    debug("WiFi not ready\n");
    if (!WiFi.connecting())
    {
      debug("Attempting to connect WiFi...\n");
      WiFi.on();
      WiFi.connect();
    }
  }
  #endif
  else
  {
    #if Wiring_Cellular
    debug("Cellular ready\n");
    #endif
    #if Wiring_WiFi
    debug("WiFi ready\n");
    #endif    
    bool blynkconnected=Blynk.connected();
    if (!blynkconnected)
    {
      debug("Attempting to connect to Blynk...\n");

      Blynk.run();
      if (!Blynk.connect())
      {
        debug("Blynk not connected\n");
      }
    }
    else
    {
      debug("Blynk connected\n");
    }

  //*** Commented out for testing without particle connected ***
    bool partconnected=Particle.connected();
    if (!partconnected)
    {
      debug("Attempting to connect to Particle...\n");
      Particle.connect();
      if (!Particle.connected())
      {
        debug("Particle not connected\n");
      } 
    }
    else
    {
      debug("Particle connected\n");
    }
  //*** Commented out for testing without particle connected ***
  }


}  

bool connected()
{
  bool connected = true;
  #if Wiring_Cellular
  if (!Cellular.ready())
  {
    debug("Cellular not ready\n");
    connected = false;
  }
  #endif
  #if Wiring_WiFi
  if (!WiFi.ready())
  {
    debug("WiFi not ready\n");
    connected = false;
  }
  #endif  
  if (!Particle.connected)
  {
    debug("Particle not connected\n");
    connected = false;
  }
  if (!Blynk.connected())
  {
    debug("Blynk not connected\n");
    connected = false;
  }
  return connected;
}

void publishGMTOffsetRequest()
{
  // Send to https://timezonedb.com webhook for gmtOffset
  debug("publishGMTOffsetRequest Device Zone value is: ");
  debug(state.deviceZone + "\n");
  switch (state.deviceZone)
  {
    case 0:
      // America/New_York
      Particle.publish("gmtOffset", "America/New_York", PRIVATE);
      break;
    case 1:
      // America/Chicago
      Particle.publish("gmtOffset", "America/Chicago", PRIVATE);
      break;
    case 2:
      // America/Phoenix
      Particle.publish("gmtOffset", "America/Phoenix", PRIVATE);
      break;
    case 3:
      // America/Los_Angeles
      Particle.publish("gmtOffset", "America/Los_Angeles", PRIVATE);
      break;
    default:
      // America/New_York
      Particle.publish("gmtOffset", "America/New_York", PRIVATE);  
  }
}

