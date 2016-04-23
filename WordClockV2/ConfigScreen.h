// ConfigScreen.h

#ifndef _CONFIGSCREEN_h
#define _CONFIGSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <LiquidCrystal.h>
#include "Clock.h"
#include "Animation.h"
#include <EEPROM.h>
#include "Word.h"

#define YEAR_MIN_VALUE 0
#define YEAR_MAX_VALUE 100

#define MONTH_MIN_VALUE 1
#define MONTH_MAX_VALUE 12

#define DAY_MAX_VALUE_L 31
#define DAY_MAX_VALUE_S 30
#define DAY_MAX_VALUE_F_L 29
#define DAY_MAX_VALUE_F_S 28
#define DAY_MIN_VALUE 1

#define HOUR_MAX_VALUE 23
#define HOUR_MIN_VALUE 0

#define MINUTE_MAX_VALUE 59
#define MINUTE_MIN_VALUE 1

#define BRIGHT_MAX_VALUE 255
#define BRIGHT_MIN_VALUE 0

#define TEMP_MIN_VALUE 0
#define TEMP_MAX_VALUE 23

#define ANIMATION_MIN_VALUE 1
#define ANIMATION_MAX_VALUE 8

#define VALUES_COUNT 9

#define ADDRESS_TEMP_1 1
#define ADDRESS_TEMP_2 2
#define ADDRESS_BRIGHT 3
#define ADDRESS_ANIMATION 4

struct Value {
private:
	Value();

protected:
	unsigned char actualValue;
	unsigned char charPosition;
	unsigned char line;
	unsigned char size;

	Value(unsigned char initialValue, unsigned char charPosition, unsigned char line, unsigned char size) {
		this->line = line;
		this->size = size;
		this->actualValue = initialValue;
		this->charPosition = charPosition;
	}

public:
	virtual void onNextValue();
	virtual void update();

	virtual unsigned short getActualValue() {
		return this->actualValue;
	};

	void setActualValue(unsigned char val) {
		this->actualValue = val;
	};

	unsigned char getCharPosition() {
		return this->charPosition;
	};

	unsigned char getLine() {
		return this->line;
	};

	unsigned char getSize() {
		return this->size;
	};
};

struct YearValue : Value{
private:
	Clock *clock;
public:
	YearValue(Clock *clock, unsigned char charPosition, unsigned char line, unsigned char size) : Value(clock->getDate24h().year - 2000, charPosition, line, size) { this->clock = clock; }

	//Restart at 2100
	void onNextValue() override {
		this->actualValue++;

		if (this->actualValue > YEAR_MAX_VALUE) {
			this->actualValue = YEAR_MIN_VALUE;
		}

		//Update year
		ts date = this->clock->getDate24h();
		date.year = this->actualValue + 2000;
		this->clock->setDate(&date);
		this->clock->update();
	}

	void update() override {
		this->actualValue = this->clock->getDate24h().year - 2000;
	}


	unsigned short getActualValue() {
		return this->actualValue + 2000;
	};
};

struct MonthValue : Value {
private:
	Clock *clock;
public:
	MonthValue(Clock *clock, unsigned char charPosition, unsigned char line, unsigned char size) : Value(clock->getDate24h().mon, charPosition, line, size) { this->clock = clock; }
	
	//Restart at 12
	void onNextValue() override {
		this->actualValue++;

		if (this->actualValue > MONTH_MAX_VALUE) {
			this->actualValue = MONTH_MIN_VALUE;
		}

		ts date = this->clock->getDate24h();
		date.mon = this->actualValue;
		this->clock->setDate(&date);
		this->clock->update();
	}

	void update() override {
		this->actualValue = this->clock->getDate24h().mon;
	}
};

struct DayValue : Value{
private:
	MonthValue *month;
	YearValue *year;
	Clock *clock;
public: 
	DayValue(Clock *clock, MonthValue *month, YearValue *year, unsigned char charPosition, unsigned char line, unsigned char size) : Value(clock->getDate24h().mday, charPosition, line, size) { this->month = month; this->year = year; this->clock = clock; }
	
	void onNextValue() override {
		this->actualValue++;

		//Depend which month is it will be resetted at one day or another
		switch (this->month->getActualValue()) {

		//Months with 31 days
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if(this->actualValue > DAY_MAX_VALUE_L) this->actualValue = DAY_MIN_VALUE;
			break;

		//Months with 30 day
		case 4: case 6: case 9: case 11:
			if (this->actualValue > DAY_MAX_VALUE_L) this->actualValue = DAY_MIN_VALUE;
			break;

		//In febrary check if the year is leap or not
		case 2:
			if (this->year->getActualValue() % 4 == 0 && (this->year->getActualValue() % 100 != 0 || this->year->getActualValue() % 400 == 0)) {
				if (this->actualValue > DAY_MAX_VALUE_F_L) this->actualValue = DAY_MIN_VALUE;//Is leap year
			}
			else {
				if (this->actualValue > DAY_MAX_VALUE_F_S) this->actualValue = DAY_MIN_VALUE;//Normal year
			}

			break;
		}

		ts date = this->clock->getDate24h();
		date.mday = this->actualValue;
		this->clock->setDate(&date);
		this->clock->update();
	}

	void update() override {
		this->actualValue = this->clock->getDate24h().mday;
	}
};

struct HourValue : Value {
private:
	Clock *clock;
public:
	HourValue(Clock *clock, unsigned char charPosition, unsigned char line, unsigned char size) : Value(clock->getDate24h().hour, charPosition, line, size) { this->clock = clock; }

	//Restart at 24
	void onNextValue() override {
		this->actualValue++;

		if (this->actualValue > HOUR_MAX_VALUE) {
			this->actualValue = HOUR_MIN_VALUE;
		}

		ts date = this->clock->getDate24h();
		date.hour = this->actualValue;
		this->clock->setDate(&date);
		this->clock->update();
	}

	void update() override {
		this->actualValue = this->clock->getDate24h().hour;
	}
};

struct MinuteValue : Value {
private:
	Clock *clock;
public:
	MinuteValue(Clock *clock, unsigned char charPosition, unsigned char line, unsigned char size) : Value(clock->getDate24h().min, charPosition, line, size) { this->clock = clock; }

	//Restart at 12
	void onNextValue() override {
		this->actualValue++;

		if (this->actualValue > MINUTE_MAX_VALUE) {
			this->actualValue = MINUTE_MIN_VALUE;
		}

		ts date = this->clock->getDate24h();
		date.min = this->actualValue;
		date.sec = 0;
		this->clock->setDate(&date);
		this->clock->update();
	}

	void update() override {
		this->actualValue = this->clock->getDate24h().min;
	}
};

struct TempoValue : Value {
private:
	unsigned char index;
public:	
	TempoValue(unsigned char index, unsigned char charPosition, unsigned char line, unsigned char size) : Value(EEPROM.read(index), charPosition, line, size) { this->index = index; }

	//Restart at 12
	void onNextValue() override {
		this->actualValue++;

		if (this->actualValue > TEMP_MAX_VALUE) {
			this->actualValue = TEMP_MIN_VALUE;
		}

		EEPROM.write(this->index, this->actualValue);
	}

	void update() override { };
};

struct BrightValue : Value {
public:
	BrightValue(unsigned char charPosition, unsigned char line, unsigned char size) : Value(EEPROM.read(ADDRESS_BRIGHT), charPosition, line, size) { }

	//Restart at 12
	void onNextValue() override {
		if (this->actualValue >= BRIGHT_MAX_VALUE) {
			this->actualValue = BRIGHT_MIN_VALUE;
		}
		else {
			this->actualValue += 5;
		}

		EEPROM.write(ADDRESS_BRIGHT, this->actualValue);
	}

	void update()override{ };
};

//TODO: TEMP MIN AND MAX
struct AnimationValue : Value {
public:
	static void loadAnimation() {
		switch (EEPROM.read(ADDRESS_ANIMATION)) {
		case 1:
			Word::changeDefaultAnimation(RandomColor);
			break;
		case 2:
			Word::changeDefaultAnimation(WholeRainbow);
			break;
		case 3:
			Word::changeDefaultAnimation(RainbowWord);
			break;
		case 4:
			Word::changeDefaultAnimation(FixedColorCyan);
			break;
		case 5:
			Word::changeDefaultAnimation(FixedColorGreen);
			break;
		case 6:
			Word::changeDefaultAnimation(FixedColorBlue);
			break;
		case 7:
			Word::changeDefaultAnimation(FixedColorRed);
			break;
		case 8:
			Word::changeDefaultAnimation(FixedColorWhite);
			break;
		}
	}

	AnimationValue(unsigned char charPosition, unsigned char line, unsigned char size) : Value(EEPROM.read(ADDRESS_ANIMATION), charPosition, line, size) { }

	//Restart at 12
	void onNextValue() override {
		this->actualValue++;

		if (this->actualValue > ANIMATION_MAX_VALUE) {
			this->actualValue = ANIMATION_MIN_VALUE;
		}

		EEPROM.write(ADDRESS_ANIMATION, this->actualValue);


		this->loadAnimation();
	}

	void update() override { };
};

class ConfigScreen {
private:
	LiquidCrystal *lcd;
	Value *values[VALUES_COUNT];
	unsigned char position = 0;
	unsigned long flickerUpdate = 0;
	unsigned long updateHour = 0;
	unsigned char backlight = 0;
	unsigned long lastUse = 0;
	bool status = false;
	bool flickerStatus = false;
	Clock *clock;

	bool powerStatus = true;
	bool temporizerStatus = true;

	void Draw(unsigned char i);
	void ClearWord(unsigned char i);
public:
	bool changeTemporizer();
	bool changePower();
	bool isTemporizerOn();
	bool isPowered();

	ConfigScreen(LiquidCrystal *lcd, Clock *clock, unsigned char pinBacklight);
	
	void NextValue();
	void ChangeValue();
	void Update();
	void TurnOn();
	void TurnOff();
	bool isOn();
};

#endif

