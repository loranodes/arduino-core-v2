#include "Arduino.h"

#include <stdint.h>

#define BASEADDR 0x50


void FRAMinit();


void FRAMwrite(uint16_t address, uint8_t data);

void FRAMwriteblock(uint16_t startAddress, uint8_t data[], uint16_t length);


uint8_t FRAMread(uint16_t address);

void FRAMreadblock(uint16_t startAddress, uint8_t buffer[], uint16_t number);


void FRAMpack(uint16_t address, void* data, uint8_t len);

uint8_t FRAMunpack(uint16_t address, void* data, uint8_t len);


uint8_t pageAddress(uint16_t dataAddress);

uint8_t wordAddress(uint16_t dataAddress);
