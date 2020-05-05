/*
********************************************************************
Library to read settings file, where settings follow "#" characters,
to allow input of settings to ES2A8 rocket datalogger project through SD card.
James Brandreth
February 2016
********************************************************************
*/
#include "Arduino.h"
#include "SD.h"
#include "Settings.h"
Settings::Settings(){
}

int Settings::initialiseOverSampleRate(){
    char marker = '#', L = 0;
    int N = 0;
    String S;
    File settingfile;
    settingfile = SD.open("settings.txt", FILE_READ);
    if (!settingfile){
        return defaultRate;
    }
    while (settingfile.read() != marker){} //Searches for first # marker
    L = settingfile.read(); // reads following character
    S = String(L);
    N = S.toInt(); // converts to int
    if (N > 7){
        N = 7; // checks value is valid, limits to maximum value of 7
    }
    N <<= 3; // left shifts by 3 bits to align with correct location in control register
    settingfile.close();
    return N;
}

int Settings::initialiseMinInterval(){
    char marker = '#', L;
    int N = 0;
    String S;
    File settingfile;
    settingfile = SD.open("settings.txt", FILE_READ);
    if (!settingfile){
        return defaultInterval;
    }
    for (int i = 0; i < 3; i++){
        while (settingfile.read() != marker){} // searches for third # marker
    }
    L = settingfile.read();
    // Reads following characters until next #:
    while (L != marker){
        S += String(L);
        L = settingfile.read();
    }
    settingfile.close();
    N = S.toInt(); //converts to int to return
    return N;
}
int Settings::initialiseStopTime(){
    char marker = '#', L;
    int N = 0;
    String S;
    File settingfile;
    settingfile = SD.open("settings.txt", FILE_READ);
    if (!settingfile){
        return defaultStopTime;
    }
    for (int i = 0; i < 5; i++){
        while (settingfile.read() != marker){} // searches for 5th # marker
    }
    L = settingfile.read();
    // Reads following characters until next #:
    while (L != marker){
        S += String(L);
        L = settingfile.read();
    }
    settingfile.close();
    N = S.toInt(); //converts to int to return
    return N;
}
