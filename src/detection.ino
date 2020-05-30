SPS30 Sensor;

void setup_sps30()
{
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

    // Delay 8 seconds or until the SPS30 is ready
    unsigned long waitForSPS30 = millis();
    while ((!Sensor.dataAvailable()) && (millis()-waitForSPS30<8100))
    {
      delay(100);
      accel_check();
    }

}

void setup_spec() {}


void setup_detection()
{
  setup_sps30();
  setup_spec();
}
