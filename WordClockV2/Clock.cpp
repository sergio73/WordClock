// 
// 
// 

#include "Clock.h"

//Have to be called Wire.begin before create the clock
Clock::Clock() {
	DS3231_init(DS3231_INTCN);
	delay(50);//Wait to be prepared the clock
	this->update();

	if (this->date.year == 2165) {
		this->date.hour = 12;
		this->date.min = 0;
		this->date.sec = 0;
		this->date.mday = 1;
		this->date.mon = 1;
		this->date.year = 2016;
		this->setDate(&this->date);
	}
}

//Must be setted in format 24h
void Clock::setDate(const ts *date) {
	DS3231_set(*date);
}

//Return date in 12h
const ts* Clock::getDate() {
	return &this->date12h;
}

//Return date in 24h
ts Clock::getDate24h() {
	return this->date;
}

void Clock::update() {
	if (millis() - this->lastUpdate > this->updateTime) {
		this->lastUpdate = millis();
		
		DS3231_get(&date);
		date12h = date;//Copy date 24h and set to 12

		if (date12h.hour > 12) {
			date12h.hour -= 12;
		}
		else if (date12h.hour == 0) {//Simulate 12 hours while is the 00
			date12h.hour = 12;
		}
	}
}

void Clock::setUpdateTime(unsigned short updateTime) {
	this->updateTime = updateTime;
}