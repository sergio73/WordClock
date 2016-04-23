// Clock.h

#ifndef _CLOCK_h
#define _CLOCK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <ds3231.h>

class Clock {
private:
	ts date;
	ts date12h;

	unsigned short updateTime = 0;
	unsigned long lastUpdate = 0;
public:
	Clock();
	void setDate(const ts *date);
	const ts* getDate();
	ts getDate24h();
	void update();
	void setUpdateTime(unsigned short);
};

#endif

