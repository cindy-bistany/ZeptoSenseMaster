/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/setup_main.ino"
void setup();
#line 1 "/home/white3/Documents/zeptive/zeptive-052020-v01/ZeptoSenseMaster/src/setup_main.ino"
#################################################################################################
#                        BEGINNING OF SETUP
#################################################################################################

void setup()
{
  CycleOnTime = millis();
  Wire.setSpeed(20000);
  // Debug console
  pinMode(D8, INPUT_PULLDOWN);
  Serial.begin(115200);
  // Used to monitor debug without USB connection
  Serial1.begin(115200);
  delay(100);

  // Buzzer connected to D7
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Wire.begin();
  delay(100);
  debug("Start Setup() function\n");

  ///////////////////////////////////////////////////////////
  // CHECK TO MAKE SURE THE EXPANDER CAN BE SEEN
  // The i2c_scanner uses the return value of
  // the Write.endTransmisstion to see if
  // a device did acknowledge to the address.
  byte expandererror, expanderaddress;

  expanderaddress = 0x20; // MCP23017 address
  Wire.beginTransmission(expanderaddress);
  expandererror = Wire.endTransmission();

  // Try again if there is an error
  if (!expandererror==0)
  {
    Wire.reset();
    Wire.beginTransmission(expanderaddress);
    expandererror = Wire.endTransmission();
  }

  // Blink LED and reset if there is an error
  if (!expandererror == 0)
  {
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
  // END CHECK
  //////////////////////////////////////////////////////////

  // Need this to switch on the power
  power.begin();
  power.setPowerON(EXT3V3,true);
  power.setPowerON(EXT5V,true);
  // Allow time to settle
  delay(100);

  uint32_t d1=0;
	fram.readData(0, (uint8_t *)&d1, sizeof(d1));
	Serial1.printlnf("d1=%u", d1);

  SleepResult result = System.sleepResult();
  switch (result.reason()) {
    case WAKEUP_REASON_NONE: {
      debug("Device did not wake up from Particle sleep\n");
      break;
    }
    case WAKEUP_REASON_PIN: {
      debug("Device was woken up by a pin\n");
      break;
    }
    case WAKEUP_REASON_RTC: {
      debug("Device was woken up by the Particle RTC (after a specified number of seconds)\n");
      break;
    }
    case WAKEUP_REASON_PIN_OR_RTC: {
      debug("Device was woken up by either a pin or the Particle RTC (after a specified number of seconds)\n");
      break;
    }
  }
  debug("Checking i2c devices...\n");

  bool i2cOK = checkI2CDevices(i2cNames, i2cAddr, i2cLength, i2cExists);

  for (size_t x=0; x<i2cLength;++x)
  {
    debug(i2cNames[x]);
    debug(": ");
    debug(i2cExists[x] + "\n");
  }
  if (!i2cOK)
  {
    StateString = "ERR";
    debug("I2C Issue\n");
    RGB.control(true);
    // the following sets the RGB LED to red
    RGB.color(255, 0, 0);
    delay(3000);
    // resume normal operation
    RGB.control(false);    
#ifdef BEEP
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
#endif
    //Generate 9 pulses on SCL to tell slave to release the bus 
    Wire.reset();     
    if (!Wire.isEnabled()) {
      Wire.begin();
    }
    Wire.end();
  }
  else
  {
    StateString = "RDY";
    debug("I2C OK\n");
    #ifdef BEEP
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
    #endif
  }
  delay(200);
  if (!i2cOK)
  {
    System.reset();
  }

  long int clockTime = rtc.rtcNow();
  debug("Before\n");
  debug(clockTime);
  debug(": \n");
  debug(String(Time.format(clockTime, TIME_FORMAT_ISO8601_FULL)) + "\n");
  if (clockTime<946684800||clockTime>4102444799)
  {
    // 2019-01-01T00:00:00+00:00 in ISO 8601
    // Actual time is not important for rtc reset but needs to be a positive unix time
    rtc.setUnixTime(1262304000);
    long int clockTime = rtc.rtcNow();
    debug("After\n");
    debug(clockTime);
    debug(": \n");
    debug(String(Time.format(clockTime, TIME_FORMAT_ISO8601_FULL)) + "\n");
  }
  Sensor.begin();
  accel.powerOn();

  // Load state
  loadState();

  //Check for first time the firmware runs
  if (state.firstRunCheck != firstRunValue)
  {
    // Then this is the first time running so set defaults
    debugf("state.firstRunCheck = %b firstRunValue = %d\n",  state.firstRunCheck, firstRunValue);
    // state.firstRunCheck = firstRunValue; Move this to the initializeAppWidget() function
    state.numberOfReadings = 24;
    state.secondsBetweenReadings = 5;
    state.zeroOff = -156;
    strcpy(state.expression,"pm1>300||pm2>300||pm4>300||pm10>300&&conc>100||temp>150");
    strcpy(state.email,"bistany@comcast.net");
    strcpy(state.batEmail,"bistany@comcast.net");
    strcpy(state.tamperEmail,"bistany@comcast.net");
    state.batThreshold = 20.0;
    state.lastAlert=false;
    state.batLastAlert=false;
    state.ActivityThreshold=100; // Which is set to 1 in Blynk
    state.buzzerTamper=true;  // Buzzer Tamper
    state.buzzerVapor=false; // Vapor Buzzer
    state.notifyVapor=true; // Vapor Notify
    state.notifyTamper=true; // Tamper Notify
    state.notifyBattery=true; // Battery Notify
    state.OnTime=0;
    // firstRun = true;
    state.deviceZone=0; //America/New_York
    // Save state
    // saveState(); // Move this to the initializeAppWidget() function
    debug("First run.\n");
    // Force a connect here for the first time the device runs
    connect();
  }
  else
  {
    debug("Not first run.\n");
    // firstRun =false;
  }


  // restart accelerometer
  setup_accel();

/////////////
  String msg = "";
  msg = msg + "\n" + "Setting Up\n" +
    iSoftwareVersion + "\n" +
    "Vsup for all sensors = " + ULP::_Vsup + "\n" +
    "Vcc for all sensors = " + ULP::_Vcc + "\n" +
    "Vref for sensor 1 = " + sensor1._Vref + "\n";
  debug(msg);
  
  //  Using resistor values from board R1, R2, R3 are for setting _Vref and Bias, while R6 sets the gain
  //  If using modified or custom boards set Vref and Gain like this
  //long int R1 = 61900, R2 = 1000, R3 = 1000000;
  //int bias = 1; //alternatively bias=-1; for negative bias.
  //sensor1.setVref(R1, R2, R3, bias); //will set the new Vref for custom sensor voltage ladder. bias is necessary to set the correct arrangement
  //sensor1._Gain = 49900; //resistor R6

  // This is not calibrated.
  sensor1._Voff = state.zeroOff;
  debug(sensor1._Voff);


  //  Vref is not necessary if zero() is called for each sensor. If you already know the sensor zero you can comment this out, and set the zero with zero1 = measured mV.
  //   debug("Vzero = ");
  //   debug(Vzero1 = sensor1.zero());   //.zero() sets and returns the baseline voltage at current temperature with only clean air prese + "\n"nt
  //   Particle.publish("Vzero", String(Vzero1));
  debugf("Tzero = %f\n", sensor1._Tz);
  
  
  //sensor1.setXSpan();                                
  //Must have previously zeroed in clean air, returns new span factor.
  //When calibrating the temperature use "LOW"/"HIGH" for the temperature range ie .setTSpan(40.2, "HIGH") where T is the current high temperature
  //sensor1.setTSpan((71 - 32.0) * 5.0 / 9.0, "LOW");

  debug("Finished Setting Up\n");
  debug("T1, mV1, C1\n");
  ////////////

  delay(300);  

  // SPS30 checks
  bool sps30OK = false;
  uint32_t sps30start = millis();

  while (!sps30OK && millis()-sps30start<8000)
  {
    if (!Sensor.begin())
    {
      debug("Unable to read SPS30 - trying again\n");
      delay(1000);
    }
    else
    {
      debug("Read SPS30\n");
      sps30OK = true;
    }
  }
  
  if (!sps30OK)
  {
    debug("Unable to read SPS30 - resetting device\n");
    delay(1000);
    System.reset();    
  }

  #ifdef Version_2
  Blynk.config(auth, "zeptosense2.blynk.cc",portBlynk);
  #else
  Blynk.config(auth, "zeptosense.blynk.cc");
  #endif

  connectWithoutWaiting();

  //*** Commented out for testing without particle connected ***
  // Subscribe to the gmtOffset response event
  Particle.subscribe("hook-response/gmtOffset", gmtOffsetHandler, MY_DEVICES);

  // Request the GMT offset if Particle is connected and it has not yet been received
  if (Particle.connected&&!gmtOffsetValid)
  {
    publishGMTOffsetRequest();
  }
  //*** Commented out for testing without particle connected ***
 
  // Delay 8 seconds or until the SPS30 is ready
  unsigned long waitForSPS30 = millis();
  while ((!Sensor.dataAvailable()) && (millis()-waitForSPS30<8100))
  {
    delay(100);
    checkAccel();
  }

  sensorTimer.setInterval((long)state.secondsBetweenReadings*1000, readSensors); //  Here you set interval and which function to call
  sensorTimer.run(); // BlynkTimer is working...

  // There is a delay in startup until the accel can be read
  // This can be optimized to a certain extent by software but the best way is to
  // use ADXL345 Int2 as well as Int1 on new adapter board to differentiate
  // between ativity and no activity interrupts
  // without having to perform I2C calls that are blocked during connection
  accelTimer.setInterval(111,checkAccel); // timer to check accel
  accelTimer.run();

  debug("Setup complete\n");
  StateString = "RDY";
  readSensors(); 
  debug("End Setup() function\n");
}
