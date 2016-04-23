#ifndef _ANIMATIONTYPE_h
#define _ANIMATIONTYPE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
enum AnimationType {
	Indefined,
	RandomColor,
	WholeRainbow,
	RainbowWord,
	RainbowLetter,
	RainbowLetterReversed,
	FixedColorBlue,
	FixedColorWhite,
	FixedColorRed,
	FixedColorGreen,
	FixedColorCyan,
};
#endif

