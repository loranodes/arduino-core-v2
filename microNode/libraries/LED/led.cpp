#include "led.h"
#include "Arduino.h"

void RGBset(enum colour col){
	digitalWrite(rPin, col & 0b100);
	digitalWrite(gPin, col & 0b010);
	digitalWrite(bPin, col & 0b001);
}

void RGBinit(){
	pinMode(rPin, OUTPUT);
	pinMode(gPin, OUTPUT);
	pinMode(bPin, OUTPUT);
}
