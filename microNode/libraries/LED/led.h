#include "Arduino.h"

#define rPin 11
#define gPin 12
#define bPin 13

enum colour {
	OFF		= 0b000,
	BLUE	= 0b001,
	GREEN	= 0b010,
	CYAN	= 0b011,
	RED		= 0b100,
	PINK	= 0b101,
	WHITE	= 0b111
};

void RGBset(enum colour col);

void RGBinit();
