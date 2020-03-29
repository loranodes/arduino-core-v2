#include <stdint.h>
#include "Arduino.h"

#include "moisture.h"
#include "fastTimer.h"

static __inline__ void syncGCLK() __attribute__ ((always_inline, unused));
static void syncGCLK(){
  while(GCLK->STATUS.bit.SYNCBUSY == 1);
}

static __inline__ void ACsync() __attribute__ ((always_inline, unused));
static void ACsync(){
  while(AC->STATUSB.bit.SYNCBUSY == 1);
}

#define LOWER_TH 10
#define UPPER_TH 40

volatile uint32_t cycles = 0;

uint32_t start = 0;
uint32_t finish = 0;

uint8_t pin;

void AC_Handler(){
  cycles += 1;
  
  uint8_t ACinterrupt = REG_AC_INTFLAG; // copy out flag reg
  AC->INTFLAG.bit.COMP0=1;        //Reset int flags

  if(ACinterrupt & 0x1){
    digitalWrite(10, HIGH);
  }
  else if(ACinterrupt & 0x2){
    digitalWrite(10, LOW);
  }
}

// datasheet pg 896 for all AC registers
void init_AC(uint8_t outPin){
  pin = outPin;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  
  // Setup the clock sources for the ADC analog and digitalWrite Pg 95
  syncGCLK();
  GCLK->CLKCTRL.reg = 0x4120;   //enable GGCLK for AC_ana, clkgen1 = 32 kHz crystal
  syncGCLK();
  GCLK->CLKCTRL.reg = 0x401F;   //enable GGCLK for AC_dig, clkgen0 = 48 MHz PLL
  syncGCLK();

  // Enable the AC clock in the power manager reg
//  PM->APBCMASK.bit.AC = 1; ? doesnt like this
  REG_PM_APBCMASK |= PM_APBCMASK_AC;

  // Disable comparator
  AC->CTRLA.bit.ENABLE=0;

  ACsync();

  AC->COMPCTRL[0].bit.HYST=1;     //Enable hysteresis Pg 909
  AC->COMPCTRL[0].bit.OUT=0;      //Output is not routed to a pin Pg 909

  AC->COMPCTRL[0].bit.MUXPOS=0x1;   //pin 0 as positive input. Pg 910
  AC->COMPCTRL[0].bit.MUXNEG=0x5;   //Vscale negative input Pg 910

  AC->COMPCTRL[0].bit.INTSEL=0x2;     //Set interrupt mode rising=0x1 falling=0x2 Pg 911

  AC->COMPCTRL[0].bit.SPEED=1;    //AC speed high=1, low=0 Pg 911
  AC->COMPCTRL[0].bit.SINGLE=0;     //Continuous measurement Pg 911

  ACsync();

  AC->COMPCTRL[1].bit.HYST=1;     //Enable hysteresis Pg 909
  AC->COMPCTRL[1].bit.OUT=0;      //Output is not routed to a pin Pg 909

  AC->COMPCTRL[1].bit.MUXPOS=0x1;   //pin 0 as positive input. Pg 910
  AC->COMPCTRL[1].bit.MUXNEG=0x5;   //Vscale negative input Pg 910

  AC->COMPCTRL[1].bit.INTSEL=0x1;     //Set interrupt mode rising=0x1 falling=0x2 Pg 911

  AC->COMPCTRL[1].bit.SPEED=1;    //AC speed high=1, low=0 Pg 911
  AC->COMPCTRL[1].bit.SINGLE=0;     //Continuous measurement Pg 911

  ACsync();

  // Enable AC interrupts
  AC->INTENSET.bit.COMP0=1; //Enable AC0 interrupt Pg 902
  AC->INTENSET.bit.COMP1=1; //Enable AC1 interrupt Pg 902

  //set AC Vscale voltages
  // Vscale = (Vdd*(value + 1)) / 64
  REG_AC_SCALER0 = LOWER_TH;
  REG_AC_SCALER1 = UPPER_TH;


  // Enable interrupt vector
  NVIC_EnableIRQ(AC_IRQn);

  ACsync();
  AC->COMPCTRL[0].bit.ENABLE=1; //Enable AC 0 Pg 908
  ACsync();
  AC->COMPCTRL[1].bit.ENABLE=1; //Enable AC 1 Pg 908
  ACsync();

  AC->CTRLA.bit.ENABLE=1;   //Enable AC unit Pg 898
  ACsync();
  AC->INTFLAG.bit.COMP0=1;  //Reset int flags
  ACsync();
  AC->INTFLAG.bit.COMP1=1;  //Reset int flags

  delay(100);
  digitalWrite(pin, HIGH);
}


void disableAC(){
  ACsync();
  AC->CTRLA.bit.ENABLE=0;   //disable AC Pg 898
  ACsync();
}


uint32_t takeSoilReading(uint16_t counts){

  init_TC();
  reset_TC();
  cycles = 0;

  while(cycles < counts){
    if(timeout_TC()){
      Serial.println("true");
      return 0;
    }
  }
  
  uint32_t time = read_TC();

  disable_TC();

  uint32_t period =  ticks_to_micros(time) / counts;
  // uint32_t cap = ((double(dur))*1000000) / double(539227);
  
  return period;

}
