/*
********************************************************************
Library to read settings file, where settings follow "#" characters,
to allow input of settings to ES2A8 rocket datalogger project through SD card.
James Brandreth
February 2016
********************************************************************
*/
#ifndef Settings_h
#define Settings_h
#include "Arduino.h"
#include "SD.h"
#define defaultInterval 100
#define defaultRate 0
class Settings{
    public:
        Settings();
        int initialiseOverSampleRate(void);
        int initialiseMinInterval(void);
        int initialiseStopTime(void);
    private:
    
};
#endif
