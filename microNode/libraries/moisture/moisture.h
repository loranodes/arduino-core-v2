#include <stdint.h>
#include "Arduino.h"

// datasheet pg 896 for all AC registers
void init_AC(uint8_t outPin);

void disableAC();

uint32_t takeSoilReading(uint16_t counts);

