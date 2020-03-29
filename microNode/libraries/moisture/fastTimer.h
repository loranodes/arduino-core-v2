#include <stdint.h>
#include "Arduino.h"


void init_TC();

void reset_TC();

uint32_t read_TC();

uint8_t timeout_TC();

void disable_TC();

uint32_t micros_to_ticks(uint32_t micros);

uint32_t ticks_to_micros(uint32_t ticks);
