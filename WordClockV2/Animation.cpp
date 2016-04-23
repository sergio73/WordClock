// 
// 
// 

#include "Animation.h"
#include "Word.h";

#pragma region Static
CRGB *Animation::leds = NULL;

void Animation::setLeds(CRGB *leds) {
	Animation::leds = leds;
}
void Animation::setActualAnimation(unsigned char animation) {
	Animation::actualAnimation = actualAnimation;
}
unsigned char Animation::getActualAnimation() {
	return Animation::actualAnimation;
}
Animation *Animation::instantiate(Word *word) {
	switch (word->getAnimationType())
	{
	case RandomColor:
		return new RandomColorAnimation(word);
	case WholeRainbow:
		return new WholeRainbowAnimation(word);
	case RainbowWord:
		return new RainbowWordAnimation(word);
	case RainbowLetter:
		return new RainbowLetterAnimation(word, false);
	case RainbowLetterReversed:
		return new RainbowLetterAnimation(word, true);
	case FixedColorCyan:
		FixedAnimation::changeColor(CRGB::Cyan);
		return new FixedAnimation(word);
	case FixedColorBlue:
		FixedAnimation::changeColor(CRGB::LightSeaGreen);
		return new FixedAnimation(word);
	case FixedColorGreen:
		FixedAnimation::changeColor(CRGB::Lime);
		return new FixedAnimation(word);
	case FixedColorRed:
		FixedAnimation::changeColor(CRGB::OrangeRed);
		return new FixedAnimation(word);
	case FixedColorWhite:
		FixedAnimation::changeColor(CRGB::White);
		return new FixedAnimation(word);
	default:
		return new RandomColorAnimation(word);
	}
}
unsigned char Animation::lastColors[LAST_COLORS_QTY];
void Animation::pushColor(unsigned char color) {
	for (unsigned char i = LAST_COLORS_QTY - 1; i > 0; i--) {
		Animation::lastColors[i] = RandomColorAnimation::lastColors[i - 1];
	}
	Animation::lastColors[0] = color;
}

bool Animation::isClose(unsigned char color) {
	for (unsigned char i = 0; i < LAST_COLORS_QTY; i++) {
		if (max(Animation::lastColors[i], color) - min(Animation::lastColors[i], color) <= DIFFERENCE_COLOR) {
			return true;
		}
	}

	return false;
}
#pragma endregion Static

void RandomColorAnimation::animate() {
	//Map from 0, 255, to 0, 300.000 -> 0-5'
	if (millis() - lastUpdate >= map(nextUpdate, 0, 255, 0, 300000)) {
		lastUpdate = millis();

		//This variable will save a minute as 50
		//           1,5'   +   0,2-1'        +   0,1-1'     +       0,2-1'        +   0-0.6'   
		nextUpdate = 75 + random8(10, 50) + random8(5, 50) + random8(15, 50) + random8(30);

		//Get a random color
		unsigned char hue = random8();

		while (this->isClose(hue)) {
			hue = random8();
		}

		this->pushColor(hue);

		CRGB color = CHSV(hue, 255, 255);

		for (unsigned short i = this->word->getStart(); i <= this->word->getEnd(); i++) {
			Animation::leds[i] = color;
		}
	}
}

unsigned char RainbowLetterAnimation::hue = 0;
void RainbowLetterAnimation::animate() {
	if (millis() - this->lastUpdate >= 50) {
		lastUpdate = millis();

		if (this->reversed) {
			for (unsigned short i = this->word->getStart(); i <= this->word->getEnd(); i++) {
				Animation::leds[word->getEnd() - i + this->word->getStart()] = CHSV(RainbowLetterAnimation::hue++ + i * 20, 255, 255);
			}
		}
		else {
			for (unsigned short i = this->word->getEnd(); i >= this->word->getStart(); i--) {
				Animation::leds[i] = CHSV(RainbowLetterAnimation::hue++ + i * 20, 255, 255);
			}
		}
	}
}

CRGB FixedAnimation::color = CRGB::Aqua;
void FixedAnimation::changeColor(CRGB color) {
	FixedAnimation::color = color;
}
void FixedAnimation::animate() {
	for (unsigned short i = this->word->getStart(); i <= this->word->getEnd(); i++) {
		Animation::leds[i] = color;
	}
}

unsigned char WholeRainbowAnimation::hue = 0;
void WholeRainbowAnimation::animate() {
	if (millis() - this->lastUpdate >= 200) {
		this->lastUpdate = millis();

		//Only update with the first letter because is always on
		if (this->word->getStart() == 0) {
			WholeRainbowAnimation::hue++;
		}

		for (unsigned short i = this->word->getStart(); i <= this->word->getEnd(); i++) {
			Animation::leds[i] = CHSV(WholeRainbowAnimation::hue, 255, 255);
		}
	}
}

void RainbowWordAnimation::animate() {
	if (millis() - this->lastUpdate >= 200) {
		this->lastUpdate = millis();

		this->hue++;

		for (unsigned short i = this->word->getStart(); i <= this->word->getEnd(); i++) {
			Animation::leds[i] = CHSV(this->hue, 255, 255);
		}
	}
}
