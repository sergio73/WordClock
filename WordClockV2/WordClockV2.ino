/*
 Name:		WordClockV2.ino
 Created:	06/02/2016 13:44:08
 Author:	Sergio
*/
//#define DEBUG

#ifdef DEBUG
	#include <SoftwareSerial.h>
	#include <MemoryFree.h>
#endif

#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "ConfigScreen.h"
#include "Animation.h"
#include <Wire.h>
#include <ds3231.h>
#include <config.h>
#include "Clock.h"
#include <FastLED.h>
#include "Word.h"

#define NUM_LEDS 76
#define DATA_PIN_LEDS 8
#define DAY_BIRTHDAY 21
#define MON_BIRTHDAY 3
#define BUTTON_UP 10
#define BUTTON_DOWN 9
#define BACKLIGHT_LED A3

static CRGB leds[NUM_LEDS];
static Word *words[WORD_COUNT];
static Clock *clock;
static ConfigScreen *configScreen;

void setup() {
#ifdef DEBUG
	Serial.begin(9600);
	Serial.println(F("Start setup"));
	Serial.print(F("freeMemory()="));
	Serial.println(freeMemory());
#endif
	FastLED.addLeds<NEOPIXEL, DATA_PIN_LEDS>(leds, NUM_LEDS);

#ifdef DEBUG
	Serial.print(F("freeMemory()="));
	Serial.println(freeMemory());
#endif

	//Turn off all leds
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB::Black;
	}

	FastLED.show();

	//Leds for words
	Word::setLeds(leds);
	Animation::setLeds(leds);
	Word::setWords(words);

#ifdef DEBUG
	Serial.print(F("freeMemory()="));
	Serial.println(freeMemory());
#endif

	#pragma region CreateWords
	//Create all words
	words[0] = new Word(0, 1);//SON
	words[1] = new Word(3, 3, new HourRange(12, 1));//LA
	words[2] = new Word(3, 4, new HourRange(1, 12));//LAS
	words[3] = new Word(5, 6, new HourRange(12, 1));//UNA

	words[4] = new Word(12, 15, new DayMonthExact(DAY_BIRTHDAY, MON_BIRTHDAY));//FELIZ
	words[4]->setAnimationType(RainbowLetter);
	words[5] = new Word(10, 11, new HourRange(1, 2));//DOS
	words[6] = new Word(7, 9, new HourRange(2, 3));//TRES

	words[7] = new Word(16, 19, new HourRange(3, 4));//CUATRO
	words[8] = new Word(20, 22, new HourRange(4, 5));//CINCO

	words[9] = new Word(28, 30, new HourRange(5, 6));//SEIS
	words[10] = new Word(26, 28, new HourRange(6, 7));//SIETE
	words[11] = new Word(23, 24, new HourRange(7, 8));//OCHO

	words[12] = new Word(31, 34, new DayMonthExact(DAY_BIRTHDAY, MON_BIRTHDAY));//CUMPLE
	words[12]->setAnimationType(RainbowLetterReversed);
	words[13] = new Word(35, 37, new HourRange(8, 9));//NUEVE

	words[14] = new Word(44, 46, new HourRange(9, 10));//DIEZ
	words[15] = new Word(41, 43, new HourRange(10, 11));//ONCE
	words[16] = new Word(38, 40, new DayMonthExact(DAY_BIRTHDAY, MON_BIRTHDAY));//AÑOS
	words[16]->setAnimationType(RainbowLetter);

	words[17] = new Word(47, 49, new HourRange(11, 12));//DOCE
	words[18] = new Word(50, 50, new MinuteRangeParcial(3, 32));//Y
	words[19] = new Word(52, 54, new MinuteRangeParcial(33, 57));//MENOS

	words[20] = new Word(58, 61, new MinuteRangeComplete(18, 22));//VEINTE
	words[21] = new Word(55, 57, new MinuteRangeComplete(8, 12));//DIEZ

	words[22] = new Word(66, 68, new MinuteRangeComplete(3, 7));//CINCO
	words[23] = new Word(62, 68, new MinuteRangeComplete(23, 27));//VEINTICINCO

	words[24] = new Word(73, 75, new MinuteRangeParcial(28, 32));//MEDIA
	words[25] = new Word(69, 72, new MinuteRangeComplete(13, 17));//CUARTO
	#pragma endregion CreateWords
	
#ifdef DEBUG
	Serial.print(F("freeMemory()="));
	Serial.println(freeMemory());
#endif
	//Load animation from config
	AnimationValue::loadAnimation();

	Wire.begin();//Connect to I2C

	//Not now, Disable internal pull up to use the external ones
	//digitalWrite(SDA, 1);
	//digitalWrite(SCL, 1);

	clock = new Clock();
	clock->setUpdateTime(1000);//Minimum update time

#ifdef DEBUG
	Serial.print(F("freeMemory()="));
	Serial.println(freeMemory());
#endif

	//Create the configScreen		
	configScreen = new ConfigScreen(new LiquidCrystal(7, 6, 5, 4, 3, 2), clock, BACKLIGHT_LED);
	
	pinMode(BUTTON_DOWN, INPUT);
	pinMode(BUTTON_UP, INPUT);

	random16_set_seed(analogRead(0));//Set a new seed. It will give different random numbers each time it is turned on. analogRead(0) read a input in a pin not used
	randomSeed(analogRead(1));//Random seed for arduino function;

#ifdef DEBUG
	//Check leds
	for (int dot = 0; dot < NUM_LEDS; dot++) {
		leds[dot] = CRGB::Blue;
		FastLED.show();
		// clear this led for the next time around the loop
		leds[dot] = CRGB::Black;
		//delay(10);
	}

	Serial.print(F("freeMemory()="));
	Serial.println(freeMemory());

	Serial.println(F("End setup"));
#endif
}

#ifdef DEBUG
unsigned long printTime = 0;
#endif

unsigned long buttonDownTimePressed = 0;
unsigned long buttonUpTimePressed = 0;
unsigned long clickButton = 0;
bool firstClickButton = true;
bool upChanged = false;
bool downChanged = false;

void buttonUp() {
	if (configScreen->isOn()) {
		configScreen->NextValue();
	}
	else {
		if (!upChanged && millis() - buttonUpTimePressed > 300) {
			configScreen->changePower();
			upChanged = true;
		}
	}
}
void buttonDown() {
	if (configScreen->isOn()) {
		configScreen->ChangeValue();
	}
	else {
		if (!downChanged && millis() - buttonDownTimePressed > 300) {
			configScreen->changeTemporizer();
			downChanged = true;
		}
	}
}
void bothButtons() {
	configScreen->TurnOn();
}
// the loop function runs over and over again until power down or reset
void loop() {
	random16_add_entropy(random());//Add entropy in each iteration to generate more random numbers.

	clock->update();

	//First update all the words and turn off the ones that are in not use
	for (int i = 0; i < WORD_COUNT; i++) {
		words[i]->update(clock->getDate());
	}

	//Words that are in use animate
	for (int i = 0; i < WORD_COUNT; i++) {
		if (words[i]->getStatus()) {
			words[i]->animate();
		}
	}

	//Reset changed so the button can not be pressed twice
	if (!digitalRead(BUTTON_DOWN) && downChanged) {
		downChanged = false;
	}
	if (!digitalRead(BUTTON_UP) && upChanged) {
		upChanged = false;
	}
	if (!digitalRead(BUTTON_DOWN)) {
		buttonDownTimePressed = millis();
	}
	if (!digitalRead(BUTTON_UP)) {
		buttonUpTimePressed = millis();
	}

	//When one of the two buttons is pressed check if the other is going to be pressed or do the action
	if ((digitalRead(BUTTON_DOWN) || digitalRead(BUTTON_UP)) && millis() - clickButton > 200) {
		clickButton = millis();

		if (configScreen->isOn() || !firstClickButton) {
			firstClickButton = true;

			if (digitalRead(BUTTON_DOWN) && digitalRead(BUTTON_UP) && !configScreen->isOn()) {
				bothButtons();
			}
			else if (digitalRead(BUTTON_DOWN) && digitalRead(BUTTON_UP) && configScreen->isOn()) {
				//do nothing
			}
			else if (digitalRead(BUTTON_DOWN)) {
				buttonDown();
			}
			else if(digitalRead(BUTTON_UP)){
				buttonUp();
			}
		}

		firstClickButton = false;
	}

	configScreen->Update();

	//If is in the range set bright
	if (configScreen->isTemporizerOn() && EEPROM.read(ADDRESS_TEMP_1) != EEPROM.read(ADDRESS_TEMP_2) && EEPROM.read(ADDRESS_TEMP_1) <= clock->getDate24h().hour && EEPROM.read(ADDRESS_TEMP_2) >= clock->getDate24h().hour) {
		FastLED.setBrightness(EEPROM.read(ADDRESS_BRIGHT));
	}
	else {
		FastLED.setBrightness(255);
	}

	if (configScreen->isPowered()) {
		FastLED.show();
	}

#ifdef DEBUG
	if (millis() - printTime > 1000) {
		printTime = millis();

		const ts *date = clock->getDate();
		Serial.print(date->mday);
		Serial.print(F("/"));
		Serial.print(date->mon);
		Serial.print(F("/"));
		Serial.print(date->year);
		Serial.print(F(" "));
		Serial.print(date->hour);
		Serial.print(F(":"));
		Serial.print(date->min);
		Serial.print(F(":"));
		Serial.println(date->sec);

		Serial.print(F("freeMemory()="));
		Serial.println(freeMemory());
	}
#endif
}
