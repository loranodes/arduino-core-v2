#include <system.h>

uint16_t readVCC(){

  // disable ADC
  while (ADC->STATUS.bit.SYNCBUSY);
  ADC->CTRLA.bit.ENABLE = 0x00;

  // internal 1V reference (default)
  analogReference(AR_INTERNAL1V0);
  // 12 bit resolution (default)
  analogWriteResolution(12);
  // MUXp 0x1B = SCALEDIOVCC/4 => connected to Vcc
  ADC->INPUTCTRL.bit.MUXPOS = 0x1B ;

  // enable ADC
  while (ADC->STATUS.bit.SYNCBUSY);
  ADC->CTRLA.bit.ENABLE = 0x01;
  
  // start conversion
  while (ADC->STATUS.bit.SYNCBUSY);
  ADC->SWTRIG.bit.START = 1;
  
  // clear the Data Ready flag
  ADC->INTFLAG.bit.RESRDY = 1;
  
  // start conversion again, reading after input change needs to be discarded
  while (ADC->STATUS.bit.SYNCBUSY);
  ADC->SWTRIG.bit.START = 1;

  // waiting for conversion to complete
  while (!ADC->INTFLAG.bit.RESRDY);
  const uint32_t valueRead = ADC->RESULT.reg;

  // disable ADC
  while (ADC->STATUS.bit.SYNCBUSY);
  ADC->CTRLA.bit.ENABLE = 0x00;

  uint32_t voltage = valueRead * 4 * 1000;
  voltage /= 4096;

  return voltage * 4;
}