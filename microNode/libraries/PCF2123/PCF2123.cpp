#include "PCF2123.h"


#define SPI_MAX_SPEED 5000000

#define REG_CTRL1_ADDR            0x00
#define REG_CTRL2_ADDR            0x01
#define REG_TIME_DATE_ADDR        0x02
#define REG_ALARM_ADDR            0x09
#define REG_OFFSET_ADDR           0x0D
#define REG_TIMER_CLKOUT_ADDR     0x0E
#define REG_COUNTDOWN_TIMER_ADDR  0x0F


PCF2123::PCF2123(uint8_t ss_){
	ss = ss_;
}


void PCF2123::begin(){
	spi_cfg = SPISettings(SPI_MAX_SPEED, MSBFIRST, SPI_MODE0);

	pinMode(ss, OUTPUT);

	SPI.begin();

  	digitalWrite(ss, LOW);
}


void PCF2123::get_time(Time *t){
	uint8_t buf[7];

	this->read_reg(REG_TIME_DATE_ADDR, buf, 7);

	t->Second = bcd_decode(buf[0] & ~0x80);
	t->Minute = bcd_decode(buf[1] & ~0x80);
	t->Hour	  = bcd_decode(buf[2] & ~0xC0);
	t->Day	  = bcd_decode(buf[3] & ~0xC0);
	t->Wday	  = bcd_decode(buf[4] & ~0xF8);
	t->Month  = bcd_decode(buf[5] & ~0xE0);
	t->Year   = bcd_decode(buf[6]);
}


void PCF2123::set_time(Time new_time){
	uint8_t buf[7];

	buf[0] = bcd_encode(new_time.Second);
	buf[1] = bcd_encode(new_time.Minute);
	buf[2] = bcd_encode(new_time.Hour);
	buf[3] = bcd_encode(new_time.Day);
	buf[4] = bcd_encode(new_time.Wday);
	buf[5] = bcd_encode(new_time.Month);
	buf[6] = bcd_encode(new_time.Year);

	this->write_reg(REG_TIME_DATE_ADDR, buf, 7);
}


void PCF2123::reset(){
	uint8_t buf = 0x58;
	this->write_reg(REG_CTRL1_ADDR, &buf, 1);
}


void PCF2123::write_reg(uint8_t addr, uint8_t *buf, uint8_t len){
	SPI.beginTransaction(spi_cfg);

	digitalWrite(ss, HIGH);

	// 0x10 required sub address
	// 0x00 setup a write
	SPI.transfer(0x10 | 0x00 | addr);

	for(uint8_t i = 0; i<len; i++){
		SPI.transfer(buf[i]);
	}

	digitalWrite(ss, LOW);
	SPI.endTransaction();
}


void PCF2123::read_reg(uint8_t addr, uint8_t *buf, uint8_t len){
	SPI.beginTransaction(spi_cfg);

	digitalWrite(ss, HIGH);

	// 0x10 required sub address
	// 0x80 setup a read
	SPI.transfer(0x10 | 0x80 | addr);

	for(uint8_t i = 0; i<len; i++){
		buf[i] = SPI.transfer(0);
	}

	digitalWrite(ss, LOW);
	SPI.endTransaction();
}


uint8_t PCF2123::bcd_decode(uint8_t bcd){
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}


uint8_t PCF2123::bcd_encode(uint8_t dec){
	return ((dec / 10) << 4) | (dec % 10);
}