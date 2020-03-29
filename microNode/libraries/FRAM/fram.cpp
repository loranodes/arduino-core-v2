#include "Arduino.h"
#include "fram.h"
#include "Wire.h"


void FRAMinit(){
    Wire.begin();
}


void FRAMwrite(uint16_t address, uint8_t data){
    Wire.beginTransmission(pageAddress(address));

    Wire.write(wordAddress(address));

    Wire.write(data);

    Wire.endTransmission(true);
}


void FRAMwriteblock(uint16_t startAddress, uint8_t data[], uint16_t length){
    Wire.beginTransmission(pageAddress(startAddress));
    Wire.write(wordAddress(startAddress));

    Wire.write(data, length);

    Wire.endTransmission(true);
}


uint8_t FRAMread(uint16_t address){
    Wire.beginTransmission(pageAddress(address));
    Wire.write(wordAddress(address));
    Wire.endTransmission(false);

    Wire.requestFrom(pageAddress(address), 1);

    uint8_t data = Wire.read();

    return data;
}


void FRAMreadblock(uint16_t startAddress, uint8_t buffer[], uint16_t number){
    Wire.beginTransmission(pageAddress(startAddress));
    Wire.write(wordAddress(startAddress));
    Wire.endTransmission(false);

    Wire.requestFrom(pageAddress(startAddress), number);

    for(int i = 0; i < number; i++){
        buffer[i] = Wire.read();
    }
}


void FRAMpack(uint16_t address, void* data, uint8_t len){
    Wire.beginTransmission(pageAddress(address));
    Wire.write(wordAddress(address));

    Wire.write((uint8_t*)data, len);
    Wire.endTransmission(true);
}


uint8_t FRAMunpack(uint16_t address, void* data, uint8_t len){
    uint8_t rc;
    uint8_t *p = (uint8_t*)data;

    Wire.beginTransmission(pageAddress(address));
    Wire.write(wordAddress(address));
    Wire.endTransmission(false);

    Wire.requestFrom(pageAddress(address), len);

    for(rc = 0, p; rc < len ; rc++, p++){
        *p = (uint8_t)Wire.read();
    }

    return rc;
}


uint8_t pageAddress(uint16_t dataAddress){
    uint8_t page = dataAddress >> 8;
    return (BASEADDR | page);
}

uint8_t wordAddress(uint16_t dataAddress){
    return (dataAddress & 0b11111111);
}
