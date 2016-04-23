#ifndef _RANGE_h
#define _RANGE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "ds3231.h"

struct TimeRange {
public:
	virtual bool isActive(const ts *date);
};

struct DayMonthExact : TimeRange {
private:
	unsigned char day;
	unsigned char month;
public:
	DayMonthExact(unsigned char day, unsigned char month) {
		this->day = day;
		this->month = month;
	}

	virtual bool isActive(const ts *date) {
		return date->mday == this->day && date->mon == this->month;
	}
};

struct HourRange : TimeRange {
private:
	unsigned char start;
	unsigned char end;
public:
	HourRange(unsigned char start, unsigned char end) {
		this->start = start;
		this->end = end;
	}

	virtual bool isActive(const ts *date) {
		bool active = false;
		if (this->start > this->end)
			active = date->hour == this->start || this->end == date->hour;
		else
			active =  date->hour >= start &&  date->hour <= end;

		if (this->start == date->hour) {
			active &= date->min >= 33;
		}
		else if (this->end == date->hour) {
			active &= date->min <= 32;
		}

		return active;
	}
};


struct MinuteRangeParcial : TimeRange {
private:
	unsigned char start;
	unsigned char end;
public:
	MinuteRangeParcial(unsigned char start, unsigned char end) {
		this->start = start;
		this->end = end;
	}

	virtual bool isActive(const ts *date) {
		return date->min >= this->start && date->min <= this->end;
	}
};

struct MinuteRangeComplete : TimeRange {
private:
	unsigned char start;
	unsigned char end;
public:
	MinuteRangeComplete(unsigned char start_1, unsigned char end_1) {
		this->start = start;
		this->end = end;
	}

	virtual bool isActive(const ts *date) {
		return (date->min >= this->start && date->min <= this->end) || (date->min >= (60 - this->end) && date->min <= (60 - this->start));
	}
};

#endif