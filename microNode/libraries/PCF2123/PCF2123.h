#include <SPI.h>
#include "Arduino.h"


struct Time{
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Wday;
	uint8_t Month;
	uint8_t Year;
};


class PCF2123{
	public:
		uint8_t bcd_decode(uint8_t bcd);

    	uint8_t bcd_encode(uint8_t dec);

    	PCF2123(uint8_t ss_);

    	void begin();

    	void get_time(Time *t);

    	void set_time(Time new_time);

	private:
		uint8_t ss;
		SPISettings   spi_cfg;

		void reset();

		void write_reg(uint8_t addr, uint8_t *buf, uint8_t len);
		void read_reg(uint8_t addr, uint8_t *buf, uint8_t len);

};