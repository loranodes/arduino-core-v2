#include "Arduino.h"
#include <stdint.h>

#include "fastTimer.h"


static __inline__ void syncGCLK() __attribute__ ((always_inline, unused));
static void syncGCLK(){
  while(GCLK->STATUS.bit.SYNCBUSY == 1);
}

static __inline__ void syncTC() __attribute__ ((always_inline, unused));
static void syncTC(){
  while(TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
}


#define TIMER_CLK 			37500
#define TIMER_PRESCALE		4


uint8_t timeout_TC(){
	if(TC5->COUNT16.INTFLAG.bit.OVF){
		TC5->COUNT16.INTFLAG.bit.OVF = 1;
		return 1;
	}
	return 0;
}

void init_TC(){
  	syncGCLK();
  	GCLK->CLKCTRL.reg = 0x411C;   //enable GGCLK for AC_ana, clkgen1 = 32 kHz crystal
  	syncGCLK();

  	TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  	syncTC();
  	while (TC5->COUNT16.CTRLA.bit.SWRST);

  	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

  	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV4 | TC_CTRLA_ENABLE;

	TC5->COUNT16.INTENSET.bit.OVF = 1;
  	syncTC();

}


void reset_TC(){
	TC5->COUNT16.COUNT.reg = 0;
	TC5->COUNT16.INTENSET.bit.OVF = 1;
	syncTC();
}


uint32_t read_TC(){
	return TC5->COUNT16.COUNT.reg;
}


void disable_TC(){
	TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
	syncTC();
}


uint32_t micros_to_ticks(uint32_t micros){
	return (micros * TIMER_CLK * TIMER_PRESCALE) / 1e6; 
}


uint32_t ticks_to_micros(uint32_t ticks){
	return (ticks * TIMER_PRESCALE * 1e6) / TIMER_CLK; 
}
