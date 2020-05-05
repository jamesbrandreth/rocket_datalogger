/*
********************************************************************
ROCKET FLIGHT DATALOGGER
ES2A8 GROUP PROJECT, GROUP CHARLIE
James Brandreth & Gayatri Kapur
February 2016
********************************************************************
Program to read pressure, temperature, altitude from an MPL3115A2
barometric pressure sensor, 3-axis acceleration from an ADXL345
accelerometer and write the data to a file on an SD card.
********************************************************************
LIBRARIES
This program uses two third-party libraries from Adafruit:
Adafruit_Sensor.hm, Adafruit_ADXL345_U.h.
It also uses an edited version of the Adafruit_MPL3115A2.h library,
which should be provided with this program.
The Settings.h library is custom-built and also should be provided.
********************************************************************
*/
#include <Wire.h>
#include <SD.h>
#include <Adafruit_MPL3115A2.h> //NB must use edited version of this library provided with this program
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Settings.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Settings set = Settings();

File flight;
float pressure, altitude, temperature, accX, accY, accZ;
int OverSampleRate, interval, stoptime;
uint32_t totalTime = 0, loopStart = 0, startTime = 0;
String line;

void setup(){
	SD.begin(4);
	baro.begin();
	accel.begin();
	accel.setRange(ADXL345_RANGE_16_G);
	OverSampleRate = set.initialiseOverSampleRate(); //READ SETTINGS FILE TO SET OVERSAMPLE RATE
	interval = set.initialiseMinInterval(); //READ SETTINGS FILE TO SET MINIMUM READING INTERVAL
	stoptime = set.initialiseStopTime();
	flight = SD.open("flight.txt", FILE_WRITE);
	//======PRINT DATA FILE HEADER======
	flight.println("ROCKET FLIGHT DATALOGGER - DATA FILE");
	flight.println("GROUP CHARLIE");
	flight.println("Oversample ratio: 2^" + String((OverSampleRate >>= 3)) + "\t Minimum reading interval: " + String(interval));
	flight.println("Time(ms) \t Pressure (pascals)\t Altitude (m) \t Temp (degreeC) \t X acceleration (m/s^2) \t Y acceleration (m/s^2) \t Z acceleration (m/s^2)");
	flight.flush();
}

void loop(){
	totalTime=millis();
	if(totalTime-loopStart > interval){
		loopStart=millis();
		//======ALTIMETER READING======
		altitude = baro.getAltitude(OverSampleRate);
		//======PRESSURE READING=======
		pressure = baro.getPressure(OverSampleRate);
		//=======TEMPERATURE READING=======
		temperature = baro.getTemperature(OverSampleRate);
		//=======ACCELEROMETER READING=======
		sensors_event_t event;
		accel.getEvent(&event);
		accX = event.acceleration.x;
		accY = event.acceleration.y;
		accZ = event.acceleration.z;
		//=======ASSEMBLE AND PRINT STRING=======
		line=String(totalTime)+"\t"+String(pressure)+"\t"+String(altitude)+"\t"+String(temperature)+"\t"+String(accX)+"\t"+String(accY)+"\t"+String(accZ);
		flight.println(line);
		flight.flush(); //SAVES LINE TO SD CARD
	}

	if((totalTime - startTime) >= stoptime)
	{
		flight.close();
		while(1){} // Stops taking readings at time defined in settings file
	}
}

