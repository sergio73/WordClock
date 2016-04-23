// Animation.h

#ifndef _ANIMATION_h
#define _ANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

//Prevent to have similar colors
#define LAST_COLORS_QTY 5
#define DIFFERENCE_COLOR 20

class Word;
#include "FastLED.h";
#include "AnimationType.h";

class Animation {
protected:
	static CRGB *leds;
	static unsigned char actualAnimation;

	Word *word;
	Animation(Word *word) { this->word = word; }

	static unsigned char lastColors[LAST_COLORS_QTY];
	void pushColor(unsigned char color);
	bool isClose(unsigned char color);
	void clearColors();
public:
	static void setLeds(CRGB *leds);
	static void setActualAnimation(unsigned char);
	static unsigned char getActualAnimation();

	virtual void animate();
	static Animation *instantiate(Word *word);
};


class RandomColorAnimation : public Animation {
private:
	unsigned long lastUpdate = 0;
	unsigned char nextUpdate = 0;
public:
	RandomColorAnimation(Word *word) : Animation(word) {};

	void animate() override;
};

class RainbowLetterAnimation : public Animation {
private:
	static unsigned char hue;
	unsigned long lastUpdate = 0;
	bool reversed = false;
public:
	RainbowLetterAnimation(Word *word, bool reversed) : Animation(word) { this->reversed = reversed; };

	void animate() override;
};

class WholeRainbowAnimation : public Animation {
private:
	static unsigned char hue;
	unsigned long lastUpdate = 0;
public:
	WholeRainbowAnimation(Word *word) : Animation(word) { };

	void animate() override;
};

class FixedAnimation : public Animation {
private:
	static CRGB color;
public:
	FixedAnimation(Word *word) : Animation(word) {};

	static void changeColor(CRGB color);
	void animate() override;
};
class RainbowWordAnimation : public Animation {
private:
	unsigned char hue;
	unsigned long lastUpdate = 0;
public:
	//Each 
	RainbowWordAnimation(Word *word) : Animation(word) {
		//Get a random color
		this->hue = random8();

		while (this->isClose(this->hue)) {
			this->hue = random8();
		}

		this->pushColor(this->hue);
	};

	void animate() override;
};
#endif
