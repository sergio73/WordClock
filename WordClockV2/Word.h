// Word.h

#ifndef _WORD_h
#define _WORD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "FastLED.h";
#include "Range.h";
#include "AnimationType.h";
#include "ds3231.h";
#include "Animation.h"

#define WORD_COUNT 26

class Word {
private:
	static Word **words;
	static CRGB *leds;

	TimeRange *timeRange = NULL;

	unsigned char start;
	unsigned char end;

	Animation *animation = NULL;
	AnimationType animType = Indefined;
	bool status = false;

public:
	static void setLeds(CRGB *leds);
	static void setWords(Word **words);
	static void changeDefaultAnimation(AnimationType animType);

	Word(unsigned char start, unsigned char end);
	Word(unsigned char start, unsigned char end, TimeRange *timeRange);

	void update(const ts *date);
	void turnOff();
	void turnOn();
	void animate();

	void setAnimation(Animation *animation);
	bool getStatus();
	AnimationType getAnimationType();
	void setAnimationType(AnimationType animType);
	unsigned char getStart();
	unsigned char getEnd();
};
#endif

