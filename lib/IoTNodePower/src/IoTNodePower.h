#ifndef IoTNodePower_h
#define IoTNodePower_h

#ifdef PARTICLE
  #include "application.h"
#else
  #include "arduino.h"
#endif //end of #ifdef PARTICLE

enum powerName {INT5V, INT12V, EXT3V3, EXT5V, EXT12V};

enum gioName {GIO1=11, GIO2, GIO3};

class IoTNodePower
    {
        public:

        IoTNodePower();

        void begin();
        void setPowerON(powerName pwrName, bool state);
        void setPullUp(gioName ioName, bool state);
        void digitalWrite(gioName ioName, bool state);
        bool digitalRead(gioName ioName);
        void tickleWatchdog();
        bool isLiPoPowered();
        bool is3AAPowered();
        bool isLiPoCharged();
        bool isLiPoCharging();
        float voltage();

        private:
    };

#endif
