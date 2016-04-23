// 
// 
// 

#include "Word.h"

#pragma region Static
CRGB *Word::leds = NULL;
Word **Word::words = NULL;

void Word::setLeds(CRGB *leds) {
	Word::leds = leds;
}
void Word::setWords(Word **words) {
	Word::words = words;
}
void Word::changeDefaultAnimation(AnimationType animType) {
	for (unsigned char i = 0; i < WORD_COUNT; i++){
		//Rainbow letters are specials
		if (Word::words[i]->animType == RainbowLetter || Word::words[i]->animType == RainbowLetterReversed) {
			continue;
		}
		else {
			Word::words[i]->animType = animType;

			//Words that are turned on change on the fly
			if (Word::words[i]->status) {
				delete Word::words[i]->animation;
				Word::words[i]->animation = Animation::instantiate(Word::words[i]);
				Word::words[i]->animation->animate();
			}
		}
	}
}
#pragma endregion Static

#pragma region Constructors
Word::Word(unsigned char start, unsigned char end) {
	this->start = start;
	this->end = end;
}
Word::Word(unsigned char start, unsigned char end, TimeRange *timeRange) {
	this->start = start;
	this->end = end;
	this->timeRange = timeRange;
}
#pragma endregion Constructors

void Word::update(const ts *date) {
	bool pass = true;

	if (this->timeRange != NULL) {
		this->timeRange->isActive(date);
	}

	if (this->status && !pass) {//Turn off
		this->turnOff();
	}
	else if (!this->status && pass){//Turn on
		this->turnOn();
	}

	this->status = pass;
}

void Word::turnOff() {
	delete this->animation;//While is off obviusly there is no animation
	this->animation = NULL;

	for (unsigned short i = this->start; i <= this->end; i++) {
		this->leds[i] = CRGB::Black;
	}
}
void Word::turnOn() {
	this->animation = Animation::instantiate(this);
}

void Word::animate() {
	this->animation->animate();
}

bool Word::getStatus() {
	return this->status;
}

void Word::setAnimation(Animation *animation) {
	delete this->animation;//Delete the previous animation
	this->animation = animation;
}

AnimationType Word::getAnimationType() {
	return this->animType;
}
void Word::setAnimationType(AnimationType animType) {
	this->animType = animType;
}
unsigned char Word::getStart() {
	return this->start;
}
unsigned char Word::getEnd() {
	return this->end;
}