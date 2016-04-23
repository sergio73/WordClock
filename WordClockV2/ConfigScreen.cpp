// 
// 
// 

#include "ConfigScreen.h"

bool ConfigScreen::changePower() {
	this->powerStatus = !this->powerStatus;

	if (!this->powerStatus) {
		FastLED.setBrightness(0);
		FastLED.show();
	}

	return this->powerStatus;
}
bool ConfigScreen::changeTemporizer() {
	this->temporizerStatus = !this->temporizerStatus;
	return this->temporizerStatus;
}
bool ConfigScreen::isPowered() {
	return this->powerStatus;
}
bool ConfigScreen::isTemporizerOn() {
	return this->temporizerStatus;
}

ConfigScreen::ConfigScreen(LiquidCrystal *lcd, Clock *clock, unsigned char pinBacklight) {
	this->lcd = lcd;
	this->clock = clock;
	this->backlight = pinBacklight;

	pinMode(pinBacklight, OUTPUT);
	digitalWrite(pinBacklight, LOW);

	YearValue *yearValue = new YearValue(clock, 6, 0, 4);
	MonthValue *monthValue = new MonthValue(clock, 3, 0, 2);
	DayValue *dayValue = new DayValue(clock, monthValue, yearValue, 0, 0, 2);
	
	this->values[0] = dayValue;
	this->values[1] = monthValue;
	this->values[2] = yearValue;
	this->values[3] = new HourValue(clock, 11, 0, 2);
	this->values[4] = new MinuteValue(clock, 14, 0, 2);
	this->values[5] = new TempoValue(ADDRESS_TEMP_1, 2, 1, 2);
	this->values[6] = new TempoValue(ADDRESS_TEMP_2, 5, 1, 2);
	this->values[7] = new BrightValue(11, 1, 3);
	this->values[8] = new AnimationValue(15, 1, 1);

	this->lcd->begin(16, 2);
	this->lcd->clear();
	this->lcd->noDisplay();

	if (EEPROM.read(ADDRESS_TEMP_1) > TEMP_MAX_VALUE) {
		EEPROM.write(ADDRESS_TEMP_1, TEMP_MIN_VALUE);
	}
	if (EEPROM.read(ADDRESS_TEMP_2) > TEMP_MAX_VALUE) {
		EEPROM.write(ADDRESS_TEMP_2, TEMP_MIN_VALUE);
	}
	if (EEPROM.read(ADDRESS_ANIMATION) > ANIMATION_MAX_VALUE) {
		EEPROM.write(ADDRESS_ANIMATION, ANIMATION_MIN_VALUE);
	}
	if (EEPROM.read(ADDRESS_BRIGHT) > BRIGHT_MAX_VALUE) {
		EEPROM.write(ADDRESS_BRIGHT, BRIGHT_MIN_VALUE);
	}
}

void ConfigScreen::NextValue() {
	//Re-Draw the last position just in case the word is been cleared
	Draw(this->position);

	position++;

	//Resed
	if (position >= VALUES_COUNT) {
		position = 0;
	}

	lastUse = millis();
	this->flickerUpdate = 0;
	this->flickerStatus = false;
}

void ConfigScreen::ChangeValue() {
	this->values[this->position]->onNextValue();
	
	//Update the value
	Draw(this->position);

	lastUse = millis();
}

void ConfigScreen::Update() {
	if (this->status) {

		//Update all option except the selected one
		if (millis() - this->updateHour > 1000) {
			this->updateHour = millis();

			for (unsigned char i = 0; i < VALUES_COUNT; i++) {
				if (i == this->position) {
					continue;
				}

				this->values[i]->update();
				this->Draw(i);
			}
		}

		//Flicker the actual word
		if (millis() - this->flickerUpdate > 1000) {
			this->flickerUpdate = millis();

			this->flickerStatus = !this->flickerStatus;

			if (this->flickerStatus) {
				this->ClearWord(this->position);
			}
			else {
				this->Draw(this->position);
			}
		}

		if (millis() - lastUse > 30000) {
			this->TurnOff();
		}
	}
}

void ConfigScreen::TurnOn() {
	this->status = true;
	this->lastUse = millis();

	this->lcd->display();
	digitalWrite(this->backlight, HIGH);

	//Draw static elements
	this->lcd->setCursor(2, 0); this->lcd->print(F("/"));
	this->lcd->setCursor(5, 0); this->lcd->print(F("/"));
	this->lcd->setCursor(13, 0); this->lcd->print(F(":"));
	this->lcd->setCursor(0, 1); this->lcd->print(F("T:"));
	this->lcd->setCursor(4, 1); this->lcd->print(F("-"));
	this->lcd->setCursor(9, 1); this->lcd->print(F("B:"));

	if (this->temporizerStatus) {
		this->lcd->setCursor(7, 1); this->lcd->print(F("V"));
	}
	else {
		this->lcd->setCursor(7, 1); this->lcd->print(F("X"));
	}

	//Draw all elements
	for (unsigned char i = 0; i < VALUES_COUNT; i++) {
		Draw(i);
	}
}

void ConfigScreen::Draw(unsigned char i) {
	Value *val = this->values[i];
	unsigned short realVal = val->getActualValue();

	this->lcd->setCursor(val->getCharPosition(), val->getLine());

	//Max size = 5
	unsigned short size = pow(10, val->getSize() - 1);
	
	if (realVal == 0) {
		size -= 1;
	}

	while (realVal < size) {
		this->lcd->print(F("0"));
		size /= 10;
	}

	
	this->lcd->print(this->values[i]->getActualValue());
}

void ConfigScreen::ClearWord(unsigned char i) {
	Value *val = this->values[i];

	//Position at start of word
	this->lcd->setCursor(val->getCharPosition(), val->getLine());

	//Print many spaces as the letters of the word
	for (unsigned char x = 0; x < val->getSize(); x++) {
		this->lcd->print(F(" "));
	}
}

void ConfigScreen::TurnOff() {
	this->status = false;
	this->lcd->clear();
	this->lcd->noDisplay();
	digitalWrite(this->backlight, LOW);
}

bool ConfigScreen::isOn() {
	return this->status;
}