#include <led.h>
#include "PCF2123.h"


/*
 * Used to set the time in the RTC on the board
 * date needs to be sent to the board in the format
 * 
 *  -58-14-15-31-2-07-18
 *  -SS-MM-HH-DD-WD-MM-YY
 *  
 *  This can be done using the linux command:
 *  $date +-%S-%M-%H-%d-%u-%m-%y > /dev/ttyACM0
 *  
 *  where ttyACM0 is the serial port of the board
 */

//Create an instance of the RTC lib
PCF2123 time(A5);

uint8_t readVal();

void setup(){
  Serial.begin(9600);
  Serial.flush();
  RGBinit();
  //Initilise the RTC
  time.begin();
}

void loop(){
  RGBset(RED);
  while(Serial.available() == 0);
  
  //Create a instance of the time struct
  Time t;
  
  //Fill the time struct with the data over serial

  t.Second = readVal();
  t.Minute = readVal();
  t.Hour   = readVal();
  t.Day    = readVal();
  t.Wday   = readVal();
  t.Month  = readVal();
  t.Year   = readVal();
  
  
  //Fetch the current time from the RTC into the struct
  time.set_time(t);
  RGBset(GREEN);
  Serial.println("Time set");
  delay(5000);
}

uint8_t readVal(){
  Serial.read();
  return Serial.parseInt();
}

